#ifndef __INTERPRETER_H
#define __INTERPRETER_H

#include "ast.hpp" // Corrected Include
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm> // for std::stol

struct RuntimeValue
{
  enum ValType
  {
    INT,
    FLOAT,
    STRING,
    BOOL,
    VOID,
    ARRAY
  } type;
  long long int_val = 0;
  double float_val = 0.0;
  std::string string_val = "";
  bool bool_val = false;
  std::vector<RuntimeValue> array_elements;

  static RuntimeValue Integer(long long v)
  {
    RuntimeValue r;
    r.type = INT;
    r.int_val = v;
    return r;
  }
  static RuntimeValue Float(double v)
  {
    RuntimeValue r;
    r.type = FLOAT;
    r.float_val = v;
    return r;
  }
  static RuntimeValue String(std::string v)
  {
    RuntimeValue r;
    r.type = STRING;
    r.string_val = v;
    return r;
  }
  static RuntimeValue Bool(bool v)
  {
    RuntimeValue r;
    r.type = BOOL;
    r.bool_val = v;
    return r;
  }
  static RuntimeValue Void()
  {
    RuntimeValue r;
    r.type = VOID;
    return r;
  }
  static RuntimeValue Array(std::vector<RuntimeValue> v)
  {
    RuntimeValue r;
    r.type = ARRAY;
    r.array_elements = v;
    return r;
  }
};

struct ReturnException
{
  RuntimeValue value;
};

// [NEW] Control Flow Exceptions
struct BreakException
{
};
struct ContinueException
{
};

class ENVIRONMENT
{
public:
  ENVIRONMENT *parent = nullptr;
  std::unordered_map<std::string, RuntimeValue> variables;
  ENVIRONMENT(ENVIRONMENT *p = nullptr) : parent(p) {}
  void define(std::string name, RuntimeValue val) { variables[name] = val; }
  void assign(std::string name, RuntimeValue val)
  {
    if (variables.count(name))
    {
      variables[name] = val;
      return;
    }
    if (parent)
    {
      parent->assign(name, val);
      return;
    }
    std::cerr << "Runtime Error: Undefined variable '" << name << "'." << std::endl;
    exit(1);
  }
  RuntimeValue get(std::string name)
  {
    if (variables.count(name))
      return variables[name];
    if (parent)
      return parent->get(name);
    std::cerr << "Runtime Error: Undefined variable '" << name << "'." << std::endl;
    exit(1);
  }
};

class INTERPRETER : public AST_VISITOR
{
private:
  ENVIRONMENT *current_environment;
  ENVIRONMENT *global_environment;
  RuntimeValue last_evaluated_value;
  std::unordered_map<std::string, FUNCTION_DECLARATION_STATEMENT *> functions;

  bool is_truthy(RuntimeValue v)
  {
    if (v.type == RuntimeValue::BOOL)
      return v.bool_val;
    if (v.type == RuntimeValue::INT)
      return v.int_val != 0;
    return false;
  }

public:
  INTERPRETER()
  {
    global_environment = new ENVIRONMENT();
    current_environment = global_environment;
  }

  void execute(std::vector<STATEMENT *> program)
  {
    try
    {
      for (auto stmt : program)
        stmt->accept(this);
    }
    catch (const ReturnException &)
    {
      std::cerr << "Error: Illegal return." << std::endl;
    }
    // Catch-all for break/continue to prevent crash if used at top level (semantic check should catch this though)
    catch (const BreakException &)
    {
    }
    catch (const ContinueException &)
    {
    }
  }

  void visit(LITERAL_EXPRESSION *expr) override
  {
    switch (expr->token.TYPE)
    {
    case TOKEN_INT_LITERAL:
      last_evaluated_value = RuntimeValue::Integer(std::stoll(expr->token.VALUE));
      break;
    case TOKEN_FLOAT_LITERAL:
      last_evaluated_value = RuntimeValue::Float(std::stod(expr->token.VALUE));
      break;
    case TOKEN_STRING_LITERAL:
      last_evaluated_value = RuntimeValue::String(expr->token.VALUE);
      break;
    case TOKEN_TRUE:
      last_evaluated_value = RuntimeValue::Bool(true);
      break;
    case TOKEN_FALSE:
      last_evaluated_value = RuntimeValue::Bool(false);
      break;
    case TOKEN_NULL:
      last_evaluated_value = RuntimeValue::Void();
      break;
    default:
      break;
    }
  }

  void visit(VARIABLE_EXPRESSION *expr) override { last_evaluated_value = current_environment->get(expr->name.VALUE); }

  void visit(VARIABLE_DECLARATION_STATEMENT *stmt) override
  {
    RuntimeValue val = RuntimeValue::Void();
    if (stmt->initializer_expression)
    {
      stmt->initializer_expression->accept(this);
      val = last_evaluated_value;
    }
    current_environment->define(stmt->name_token.VALUE, val);
  }

  void visit(ASSIGNMENT_EXPRESSION *expr) override
  {
    expr->value_expression->accept(this);
    current_environment->assign(expr->variable_name.VALUE, last_evaluated_value);
  }

  // FINAL VERSION: Supports Int, Float, Bool, Byte, Short, Long, Double
  void visit(BINARY_EXPRESSION *expr) override
  {
    expr->left_operand->accept(this);
    RuntimeValue left = last_evaluated_value;
    expr->right_operand->accept(this);
    RuntimeValue right = last_evaluated_value;

    // 1. BOOLEAN COMPARISON FIX (Handle == and != for booleans)
    if (left.type == RuntimeValue::BOOL && right.type == RuntimeValue::BOOL)
    {
      if (expr->operator_token.TYPE == TOKEN_DOUBLE_EQUALS)
      {
        last_evaluated_value = RuntimeValue::Bool(left.bool_val == right.bool_val);
        return;
      }
      if (expr->operator_token.TYPE == TOKEN_NOT_EQUALS)
      {
        last_evaluated_value = RuntimeValue::Bool(left.bool_val != right.bool_val);
        return;
      }
      // Booleans don't support >, <, +, etc.
      return;
    }

    // 2. STRING CONCATENATION
    if (expr->operator_token.TYPE == TOKEN_PLUS)
    {
      if (left.type == RuntimeValue::STRING || right.type == RuntimeValue::STRING)
      {
        std::string l_str = (left.type == RuntimeValue::STRING) ? left.string_val : (left.type == RuntimeValue::INT ? std::to_string(left.int_val) : (left.type == RuntimeValue::BOOL ? (left.bool_val ? "true" : "false") : std::to_string(left.float_val)));
        std::string r_str = (right.type == RuntimeValue::STRING) ? right.string_val : (right.type == RuntimeValue::INT ? std::to_string(right.int_val) : (right.type == RuntimeValue::BOOL ? (right.bool_val ? "true" : "false") : std::to_string(right.float_val)));
        last_evaluated_value = RuntimeValue::String(l_str + r_str);
        return;
      }
    }

    // 3. NUMERIC MATH (Handles Byte, Short, Int, Long, Float, Double)
    // We promote everything to 'double' for calculation if one side is float/double
    // We promote everything to 'long long' if both sides are integers

    double l_val = (left.type == RuntimeValue::INT) ? (double)left.int_val : left.float_val;
    double r_val = (right.type == RuntimeValue::INT) ? (double)right.int_val : right.float_val;
    bool are_ints = (left.type == RuntimeValue::INT && right.type == RuntimeValue::INT);

    switch (expr->operator_token.TYPE)
    {
    case TOKEN_PLUS:
      if (are_ints)
        last_evaluated_value = RuntimeValue::Integer(left.int_val + right.int_val);
      else
        last_evaluated_value = RuntimeValue::Float(l_val + r_val);
      break;

    case TOKEN_MINUS:
      if (are_ints)
        last_evaluated_value = RuntimeValue::Integer(left.int_val - right.int_val);
      else
        last_evaluated_value = RuntimeValue::Float(l_val - r_val);
      break;

    case TOKEN_ASTERISK:
      if (are_ints)
        last_evaluated_value = RuntimeValue::Integer(left.int_val * right.int_val);
      else
        last_evaluated_value = RuntimeValue::Float(l_val * r_val);
      break;

    case TOKEN_SLASH:
      if (r_val == 0)
      {
        std::cerr << "Runtime Error: Division by zero." << std::endl;
        exit(1);
      }
      if (are_ints)
        last_evaluated_value = RuntimeValue::Integer(left.int_val / right.int_val);
      else
        last_evaluated_value = RuntimeValue::Float(l_val / r_val);
      break;

    case TOKEN_PERCENT:
      if (are_ints)
        last_evaluated_value = RuntimeValue::Integer(left.int_val % right.int_val);
      else
      {
        std::cerr << "Runtime Error: Modulo on floats not supported." << std::endl;
        exit(1);
      }
      break;

    case TOKEN_GREATER_THAN:
      last_evaluated_value = RuntimeValue::Bool(l_val > r_val);
      break;
    case TOKEN_LESS_THAN:
      last_evaluated_value = RuntimeValue::Bool(l_val < r_val);
      break;
    case TOKEN_GREATER_EQUAL:
      last_evaluated_value = RuntimeValue::Bool(l_val >= r_val);
      break;
    case TOKEN_LESS_EQUAL:
      last_evaluated_value = RuntimeValue::Bool(l_val <= r_val);
      break;
    case TOKEN_DOUBLE_EQUALS:
      last_evaluated_value = RuntimeValue::Bool(l_val == r_val);
      break;
    case TOKEN_NOT_EQUALS:
      last_evaluated_value = RuntimeValue::Bool(l_val != r_val);
      break;
    default:
      break;
    }
  }
  // [NEW] Bitwise Implementation
  void visit(BITWISE_EXPRESSION *expr) override
  {
    expr->left_operand->accept(this);
    long long left = last_evaluated_value.int_val; // Assumes Type Checker guaranteed ints
    expr->right_operand->accept(this);
    long long right = last_evaluated_value.int_val;

    switch (expr->operator_token.TYPE)
    {
    case TOKEN_BITWISE_AND:
      last_evaluated_value = RuntimeValue::Integer(left & right);
      break;
    case TOKEN_BITWISE_OR:
      last_evaluated_value = RuntimeValue::Integer(left | right);
      break;
    case TOKEN_BITWISE_XOR:
      last_evaluated_value = RuntimeValue::Integer(left ^ right);
      break;
    case TOKEN_LEFT_SHIFT:
      last_evaluated_value = RuntimeValue::Integer(left << right);
      break;
    case TOKEN_RIGHT_SHIFT:
      last_evaluated_value = RuntimeValue::Integer(left >> right);
      break;
    default:
      break;
    }
  }

  // [NEW] Increment/Decrement Implementation
  void visit(INCREMENT_EXPRESSION *expr) override
  {
    VARIABLE_EXPRESSION *varExpr = dynamic_cast<VARIABLE_EXPRESSION *>(expr->variable);
    RuntimeValue currentVal = current_environment->get(varExpr->name.VALUE);

    long long original = currentVal.int_val; // Assuming Int for simplicity
    long long updated = (expr->operator_token.TYPE == TOKEN_INCREMENT) ? original + 1 : original - 1;

    current_environment->assign(varExpr->name.VALUE, RuntimeValue::Integer(updated));

    // Prefix returns new value, Postfix returns old value
    last_evaluated_value = RuntimeValue::Integer(expr->is_prefix ? updated : original);
  }

  void visit(PRINT_STATEMENT *stmt) override
  {
    stmt->expression->accept(this);
    if (last_evaluated_value.type == RuntimeValue::INT)
      std::cout << last_evaluated_value.int_val << std::endl;
    else if (last_evaluated_value.type == RuntimeValue::FLOAT)
      std::cout << last_evaluated_value.float_val << std::endl;
    else if (last_evaluated_value.type == RuntimeValue::STRING)
      std::cout << last_evaluated_value.string_val << std::endl;
    else if (last_evaluated_value.type == RuntimeValue::BOOL)
      std::cout << (last_evaluated_value.bool_val ? "true" : "false") << std::endl;
    else if (last_evaluated_value.type == RuntimeValue::ARRAY)
      std::cout << "[Array]" << std::endl;
  }

  void visit(BLOCK_STATEMENT *stmt) override
  {
    ENVIRONMENT *prev = current_environment;
    current_environment = new ENVIRONMENT(prev);
    for (auto s : stmt->statements)
      s->accept(this);
    current_environment = prev;
  }

  void visit(IF_STATEMENT *stmt) override
  {
    stmt->condition_expression->accept(this);
    if (is_truthy(last_evaluated_value))
      stmt->then_branch_statement->accept(this);
    else if (stmt->else_branch_statement)
      stmt->else_branch_statement->accept(this);
  }

  // [NEW] Switch Statement
  void visit(SWITCH_STATEMENT *stmt) override
  {
    stmt->value->accept(this);
    RuntimeValue target = last_evaluated_value;

    bool matched = false;
    for (auto &c : stmt->cases)
    {
      if (c.condition)
      {
        c.condition->accept(this);
        // Simple equality check
        if (last_evaluated_value.type == RuntimeValue::INT && target.type == RuntimeValue::INT)
        {
          if (last_evaluated_value.int_val == target.int_val)
            matched = true;
        }
        else if (last_evaluated_value.type == RuntimeValue::STRING && target.type == RuntimeValue::STRING)
        {
          if (last_evaluated_value.string_val == target.string_val)
            matched = true;
        } // ... Add float check ...
      }
      else
      {
        // Default case matches if nothing else did
        if (!matched)
          matched = true;
      }

      if (matched)
      {
        for (auto s : c.statements)
          s->accept(this);
        return; // Break switch after one successful case block
      }
    }
  }

  // [MODIFIED] Added Try-Catch for Break/Continue
  void visit(WHILE_STATEMENT *stmt) override
  {
    while (true)
    {
      stmt->condition_expression->accept(this);
      if (!is_truthy(last_evaluated_value))
        break;

      try
      {
        stmt->body_statement->accept(this);
      }
      catch (BreakException)
      {
        break;
      }
      catch (ContinueException)
      {
        continue;
      }
    }
  }

  // [NEW] Loop Controls
  void visit(BREAK_STATEMENT *stmt) override { throw BreakException(); }
  void visit(CONTINUE_STATEMENT *stmt) override { throw ContinueException(); }

  // [NEW] For Statement - Dead code technically, but required for compilation
  void visit(FOR_STATEMENT *stmt) override
  {
    ENVIRONMENT *prev = current_environment;
    current_environment = new ENVIRONMENT(prev); // Scope for initializer

    // 1. Run Initializer
    if (stmt->initializer)
      stmt->initializer->accept(this);

    while (true)
    {
      // 2. Check Condition
      if (stmt->condition)
      {
        stmt->condition->accept(this);
        if (!is_truthy(last_evaluated_value))
          break;
      }

      // 3. Run Body
      try
      {
        stmt->body->accept(this);
      }
      catch (const BreakException &)
      {
        break; // Stop loop entirely
      }
      catch (const ContinueException &)
      {
        // Catch it, but DO NOTHING.
        // Just let execution fall through to step 4 (Increment).
      }

      // 4. Run Increment (This runs even after continue!)
      if (stmt->increment)
        stmt->increment->accept(this);
    }

    current_environment = prev; // Cleanup scope
  }

  void visit(FUNCTION_DECLARATION_STATEMENT *stmt) override { functions[stmt->name_token.VALUE] = stmt; }

  void visit(CALL_EXPRESSION *expr) override
  {
    VARIABLE_EXPRESSION *func_var = dynamic_cast<VARIABLE_EXPRESSION *>(expr->callee);
    FUNCTION_DECLARATION_STATEMENT *func = functions[func_var->name.VALUE];
    std::vector<RuntimeValue> args;
    for (auto arg : expr->arguments)
    {
      arg->accept(this);
      args.push_back(last_evaluated_value);
    }
    ENVIRONMENT *prev = current_environment;
    current_environment = new ENVIRONMENT(global_environment);
    for (size_t i = 0; i < func->parameters.size(); i++)
      current_environment->define(func->parameters[i].name_token.VALUE, args[i]);
    try
    {
      func->body_block->accept(this);
      last_evaluated_value = RuntimeValue::Void();
    }
    catch (const ReturnException &ret)
    {
      last_evaluated_value = ret.value;
    }
    current_environment = prev;
  }

  void visit(RETURN_STATEMENT *stmt) override
  {
    RuntimeValue val = RuntimeValue::Void();
    if (stmt->value_expression)
    {
      stmt->value_expression->accept(this);
      val = last_evaluated_value;
    }
    throw ReturnException{val};
  }

  // [MODIFIED] Logic for Auto-Conversion
  void visit(INPUT_EXPRESSION *expr) override
  {
    if (expr->prompt_expression)
    {
      expr->prompt_expression->accept(this);
      std::cout << last_evaluated_value.string_val;
    }
    std::string line;
    std::getline(std::cin, line);

    // Auto-Conversion Logic
    // Try Int
    try
    {
      size_t idx;
      long long i = std::stoll(line, &idx);
      if (idx == line.length())
      { // Successfully parsed whole string
        last_evaluated_value = RuntimeValue::Integer(i);
        return;
      }
    }
    catch (...)
    {
    }

    // Try Float
    try
    {
      size_t idx;
      double d = std::stod(line, &idx);
      if (idx == line.length())
      {
        last_evaluated_value = RuntimeValue::Float(d);
        return;
      }
    }
    catch (...)
    {
    }

    // Fallback to String
    last_evaluated_value = RuntimeValue::String(line);
  }

  void visit(ARRAY_LITERAL_EXPRESSION *expr) override
  {
    std::vector<RuntimeValue> elements;
    for (auto el : expr->elements)
    {
      el->accept(this);
      elements.push_back(last_evaluated_value);
    }
    last_evaluated_value = RuntimeValue::Array(elements);
  }

  void visit(ARRAY_ACCESS_EXPRESSION *expr) override
  {
    expr->array_expression->accept(this);
    RuntimeValue arr = last_evaluated_value;
    expr->index_expression->accept(this);
    RuntimeValue idx = last_evaluated_value;
    if (arr.type != RuntimeValue::ARRAY)
    {
      std::cerr << "Not an array." << std::endl;
      exit(1);
    }
    if (idx.type != RuntimeValue::INT)
    {
      std::cerr << "Index not int." << std::endl;
      exit(1);
    }
    if (idx.int_val < 0 || idx.int_val >= arr.array_elements.size())
    {
      std::cerr << "Index out of bounds." << std::endl;
      exit(1);
    }
    last_evaluated_value = arr.array_elements[idx.int_val];
  }

  void visit(ARRAY_ASSIGNMENT_EXPRESSION *expr) override
  {
    // 1. Evaluate Array (target)
    expr->array_expression->accept(this);
    RuntimeValue arr_val = last_evaluated_value; // This is a COPY of the struct

    // We need a pointer to the ACTUAL variable in the environment to modify it.
    // This part is tricky because 'arr_val' is just a value.
    // Simplified approach: Re-fetch variable by name if the expression is a variable.

    VARIABLE_EXPRESSION *varExpr = dynamic_cast<VARIABLE_EXPRESSION *>(expr->array_expression);
    if (!varExpr)
    {
      std::cerr << "Runtime Error: Direct assignment to non-variable arrays not supported." << std::endl;
      exit(1);
    }

    // 2. Evaluate Index
    expr->index_expression->accept(this);
    RuntimeValue idx_val = last_evaluated_value;

    // 3. Evaluate Value
    expr->value_expression->accept(this);
    RuntimeValue assign_val = last_evaluated_value;

    // 4. Perform Update
    // Get the ACTUAL array from environment
    RuntimeValue currentArr = current_environment->get(varExpr->name.VALUE);

    if (idx_val.int_val < 0 || idx_val.int_val >= currentArr.array_elements.size())
    {
      std::cerr << "Runtime Error: Array index out of bounds." << std::endl;
      exit(1);
    }

    // Update the element
    currentArr.array_elements[idx_val.int_val] = assign_val;

    // Write it back to environment
    current_environment->assign(varExpr->name.VALUE, currentArr);

    last_evaluated_value = assign_val;
  }
  void visit(EXPRESSION_STATEMENT *stmt) override { stmt->expression->accept(this); }
  // CORRECTED: Real Logic for && and ||
  void visit(LOGICAL_EXPRESSION *expr) override
  {
    expr->left_operand->accept(this);
    RuntimeValue left = last_evaluated_value;

    if (expr->operator_token.TYPE == TOKEN_AND)
    {
      // Short-circuit: if left is false, return false
      if (!is_truthy(left))
      {
        last_evaluated_value = RuntimeValue::Bool(false);
      }
      else
      {
        expr->right_operand->accept(this);
        last_evaluated_value = RuntimeValue::Bool(is_truthy(last_evaluated_value));
      }
    }
    else if (expr->operator_token.TYPE == TOKEN_OR)
    {
      // Short-circuit: if left is true, return true
      if (is_truthy(left))
      {
        last_evaluated_value = RuntimeValue::Bool(true);
      }
      else
      {
        expr->right_operand->accept(this);
        last_evaluated_value = RuntimeValue::Bool(is_truthy(last_evaluated_value));
      }
    }
  }
  void visit(UNARY_EXPRESSION *expr) override
  {
    expr->right_operand->accept(this);
    if (expr->operator_token.TYPE == TOKEN_MINUS)
    {
      if (last_evaluated_value.type == RuntimeValue::INT)
        last_evaluated_value.int_val *= -1;
      else if (last_evaluated_value.type == RuntimeValue::FLOAT)
        last_evaluated_value.float_val *= -1.0;
    }
  }
};

#endif