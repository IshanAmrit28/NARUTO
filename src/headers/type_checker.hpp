#ifndef __TYPE_CHECKER_H
#define __TYPE_CHECKER_H

#include "ast.hpp" // Corrected include
#include <unordered_map>
#include <vector>
#include <iostream>
#include <set>

class TYPE_CHECKER : public AST_VISITOR
{
private:
  std::vector<std::unordered_map<std::string, std::string>> scope_stack;
  std::unordered_map<std::string, std::string> function_signatures;
  std::string last_evaluated_type;
  std::string current_function_return_type;
  int loop_depth = 0; // To track if break/continue is valid

  void enter_new_scope() { scope_stack.push_back({}); }
  void exit_current_scope() { scope_stack.pop_back(); }

  void declare_variable(std::string name, std::string type)
  {
    if (scope_stack.back().count(name))
    {
      std::cerr << "Semantic Error: Variable '" << name << "' already declared in this scope." << std::endl;
      exit(1);
    }
    scope_stack.back()[name] = type;
  }

  std::string lookup_variable(std::string name)
  {
    for (int i = scope_stack.size() - 1; i >= 0; i--)
    {
      if (scope_stack[i].count(name))
        return scope_stack[i][name];
    }
    std::cerr << "Semantic Error: Undefined variable '" << name << "'." << std::endl;
    exit(1);
  }

  // --- TYPE PROMOTION HELPERS ---

  // Rank types by size/precision
  int get_type_rank(std::string type)
  {
    if (type == "byte")
      return 1;
    if (type == "short")
      return 2;
    if (type == "int")
      return 3;
    if (type == "long")
      return 4;
    if (type == "float")
      return 5;
    if (type == "double")
      return 6;
    return 0; // Not a numeric type (string, bool, void, etc.)
  }

  bool is_numeric(std::string type) { return get_type_rank(type) > 0; }

  // Logic: Can we assign 'source' to 'target'?
  // e.g., int -> float (OK), float -> int (Error)
  // CORRECTED: Allow assigning Int literals to Byte/Short/Long/Double
  bool can_assign(std::string target, std::string source)
  {
    if (target == source)
      return true;
    if (source == "dynamic_input")
      return true;
    if (target == "string" || source == "string")
      return false;

    // [FIX] Allow int to be assigned to byte/short (for literals like: byte b = 10;)
    if (source == "int" && (target == "byte" || target == "short" || target == "long" || target == "double" || target == "float"))
    {
      return true;
    }
    // [FIX] Allow float to double
    if (source == "float" && target == "double")
      return true;

    int t_rank = get_type_rank(target);
    int s_rank = get_type_rank(source);

    if (t_rank > 0 && s_rank > 0)
    {
      return t_rank >= s_rank;
    }
    return false;
  }

  // Logic: What is the result of 'left OP right'?
  // e.g., int + double -> double
  std::string get_promoted_type(std::string left, std::string right)
  {
    int l_rank = get_type_rank(left);
    int r_rank = get_type_rank(right);

    if (l_rank == 0 || r_rank == 0)
      return "error"; // Non-numeric math

    // Return the larger type
    if (l_rank >= r_rank)
      return left;
    return right;
  }

public:
  TYPE_CHECKER() { enter_new_scope(); }

  void analyze(std::vector<STATEMENT *> program)
  {
    // Pre-scan functions to allow recursive calls
    for (auto statement : program)
    {
      if (auto func = dynamic_cast<FUNCTION_DECLARATION_STATEMENT *>(statement))
      {
        function_signatures[func->name_token.VALUE] = func->return_type_token.VALUE;
      }
    }
    for (auto statement : program)
      statement->accept(this);
  }

  // --- VISITOR IMPLEMENTATIONS ---

  void visit(VARIABLE_DECLARATION_STATEMENT *statement) override
  {
    std::string target_type = statement->type_token.VALUE;

    // Check initialization compatibility
    if (statement->initializer_expression)
    {
      statement->initializer_expression->accept(this);
      std::string expr_type = last_evaluated_type;

      // Special case for Empty Arrays
      if (expr_type == "array" && target_type.find("[]") != std::string::npos)
      {
        // Allowed: int[] x = [];
      }
      // Standard Type Check
      else if (!can_assign(target_type, expr_type))
      {
        std::cerr << "Type Error: Cannot initialize '" << target_type << "' with '" << expr_type << "'." << std::endl;
        exit(1);
      }
    }
    declare_variable(statement->name_token.VALUE, target_type);
  }

  // CORRECTED: Allow String Concatenation with Numbers
  void visit(BINARY_EXPRESSION *expr) override
  {
    expr->left_operand->accept(this);
    std::string left = last_evaluated_type;
    expr->right_operand->accept(this);
    std::string right = last_evaluated_type;

    // 1. COMPARISON (==, !=, <, >, etc.)
    if (expr->operator_token.TYPE >= TOKEN_DOUBLE_EQUALS && expr->operator_token.TYPE <= TOKEN_GREATER_EQUAL)
    {
      if (left != right && !(is_numeric(left) && is_numeric(right)))
      {
        std::cerr << "Type Error: Cannot compare '" << left << "' and '" << right << "'." << std::endl;
        exit(1);
      }
      last_evaluated_type = "bool";
      return;
    }

    // 2. STRING CONCATENATION (+)
    // FIX: Allow 'string' + Any or Any + 'string'
    if (expr->operator_token.TYPE == TOKEN_PLUS)
    {
      if (left == "string" || right == "string")
      {
        last_evaluated_type = "string";
        return;
      }
    }

    // 3. NUMERIC MATH (+, -, *, /, %)
    if (!is_numeric(left) || !is_numeric(right))
    {
      std::cerr << "Type Error: Binary operation '" << expr->operator_token.VALUE
                << "' requires numeric operands. Got '" << left << "' and '" << right << "'." << std::endl;
      exit(1);
    }

    // Implicit Promotion (e.g., int + float -> float)
    last_evaluated_type = get_promoted_type(left, right);
  }

  // --- NEW: Bitwise Logic (&, |, ^, <<, >>) ---
  void visit(BITWISE_EXPRESSION *expr) override
  {
    expr->left_operand->accept(this);
    std::string left = last_evaluated_type;
    expr->right_operand->accept(this);
    std::string right = last_evaluated_type;

    // Bitwise ops generally only work on integers (byte, short, int, long)
    // Floats usually don't support bitwise ops directly in C-like languages
    if (get_type_rank(left) > 4 || get_type_rank(right) > 4)
    { // 4 is long
      std::cerr << "Type Error: Bitwise operators require integer types." << std::endl;
      exit(1);
    }

    // Promote result to the larger integer type
    last_evaluated_type = get_promoted_type(left, right);
  }

  // --- NEW: Increment/Decrement (++, --) ---
  void visit(INCREMENT_EXPRESSION *expr) override
  {
    expr->variable->accept(this);
    if (!is_numeric(last_evaluated_type))
    {
      std::cerr << "Type Error: Increment/Decrement requires numeric variable." << std::endl;
      exit(1);
    }
    // Result type remains the same (e.g., int++ is int)
  }
  void visit(ARRAY_ASSIGNMENT_EXPRESSION *expr) override
  {
    expr->array_expression->accept(this);
    std::string arr_type = last_evaluated_type;

    // Check if it's actually an array
    if (arr_type.length() < 3 || arr_type.substr(arr_type.length() - 2) != "[]")
    {
      std::cerr << "Type Error: Cannot assign to non-array type." << std::endl;
      exit(1);
    }
    std::string elem_type = arr_type.substr(0, arr_type.length() - 2);

    // Check Index
    expr->index_expression->accept(this);
    if (last_evaluated_type != "int")
    {
      std::cerr << "Type Error: Array index must be int." << std::endl;
      exit(1);
    }

    // Check Value
    expr->value_expression->accept(this);
    if (!can_assign(elem_type, last_evaluated_type))
    {
      std::cerr << "Type Error: Cannot assign '" << last_evaluated_type << "' to array of '" << elem_type << "'." << std::endl;
      exit(1);
    }
    // Result of assignment is the value
    last_evaluated_type = elem_type;
  }

  void visit(ASSIGNMENT_EXPRESSION *expr) override
  {
    std::string var_type = lookup_variable(expr->variable_name.VALUE);
    expr->value_expression->accept(this);
    std::string val_type = last_evaluated_type;

    if (!can_assign(var_type, val_type))
    {
      std::cerr << "Type Error: Cannot assign '" << val_type << "' to variable of type '" << var_type << "'." << std::endl;
      exit(1);
    }
    // Assignment expression evaluates to the assigned value's type
    last_evaluated_type = val_type;
  }

  void visit(LITERAL_EXPRESSION *expr) override
  {
    switch (expr->token.TYPE)
    {
    case TOKEN_INT_LITERAL:
      last_evaluated_type = "int";
      break;
    case TOKEN_FLOAT_LITERAL:
      last_evaluated_type = "float";
      break; // or double depending on preference
    case TOKEN_STRING_LITERAL:
      last_evaluated_type = "string";
      break;
    case TOKEN_CHAR_LITERAL:
      last_evaluated_type = "char";
      break;
    case TOKEN_TRUE:
    case TOKEN_FALSE:
      last_evaluated_type = "bool";
      break;
    case TOKEN_NULL:
      last_evaluated_type = "void";
      break;
    default:
      last_evaluated_type = "unknown";
    }
  }

  void visit(VARIABLE_EXPRESSION *expr) override { last_evaluated_type = lookup_variable(expr->name.VALUE); }

  void visit(BLOCK_STATEMENT *statement) override
  {
    enter_new_scope();
    for (auto s : statement->statements)
      s->accept(this);
    exit_current_scope();
  }

  void visit(IF_STATEMENT *statement) override
  {
    statement->condition_expression->accept(this);
    if (last_evaluated_type != "bool")
    {
      std::cerr << "Type Error: 'if' condition must be 'bool', got '" << last_evaluated_type << "'." << std::endl;
      exit(1);
    }
    statement->then_branch_statement->accept(this);
    if (statement->else_branch_statement)
      statement->else_branch_statement->accept(this);
  }

  // --- NEW: Switch Statement ---
  void visit(SWITCH_STATEMENT *statement) override
  {
    statement->value->accept(this);
    std::string switch_type = last_evaluated_type;

    for (auto &c : statement->cases)
    {
      if (c.condition)
      { // Check 'case X:' types
        c.condition->accept(this);
        if (last_evaluated_type != switch_type)
        {
          // Allow strict matching for switches usually
          std::cerr << "Type Error: Case type '" << last_evaluated_type << "' does not match Switch type '" << switch_type << "'." << std::endl;
          exit(1);
        }
      }
      enter_new_scope(); // Cases usually have scopes
      for (auto s : c.statements)
        s->accept(this);
      exit_current_scope();
    }
  }

  void visit(WHILE_STATEMENT *statement) override
  {
    statement->condition_expression->accept(this);
    if (last_evaluated_type != "bool")
    {
      std::cerr << "Type Error: 'while' condition must be 'bool', got '" << last_evaluated_type << "'." << std::endl;
      exit(1);
    }
    loop_depth++;
    statement->body_statement->accept(this);
    loop_depth--;
  }

  // --- NEW: Loop Controls ---
  void visit(BREAK_STATEMENT *statement) override
  {
    if (loop_depth == 0)
    {
      std::cerr << "Semantic Error: 'break' outside of loop." << std::endl;
      exit(1);
    }
  }
  void visit(CONTINUE_STATEMENT *statement) override
  {
    if (loop_depth == 0)
    {
      std::cerr << "Semantic Error: 'continue' outside of loop." << std::endl;
      exit(1);
    }
  }

  // [NEW] Added visitor for FOR_STATEMENT to satisfy AST_VISITOR interface.
  // The parser desugars this, but we must implement it.
  void visit(FOR_STATEMENT *statement) override
  {
    // Logic same as While essentially, but simpler just to check body
    enter_new_scope();
    loop_depth++;
    statement->body->accept(this);
    loop_depth--;
    exit_current_scope();
  }

  void visit(FUNCTION_DECLARATION_STATEMENT *statement) override
  {
    enter_new_scope();
    current_function_return_type = statement->return_type_token.VALUE;
    for (auto p : statement->parameters)
      declare_variable(p.name_token.VALUE, p.type_token.VALUE);
    statement->body_block->accept(this);
    exit_current_scope();
  }

  void visit(RETURN_STATEMENT *statement) override
  {
    if (statement->value_expression)
    {
      statement->value_expression->accept(this);
      if (!can_assign(current_function_return_type, last_evaluated_type))
      {
        std::cerr << "Type Error: Return type mismatch. Expected '" << current_function_return_type << "', got '" << last_evaluated_type << "'." << std::endl;
        exit(1);
      }
    }
    else if (current_function_return_type != "void")
    {
      std::cerr << "Type Error: Non-void function must return a value." << std::endl;
      exit(1);
    }
  }

  void visit(ARRAY_LITERAL_EXPRESSION *expr) override
  {
    if (expr->elements.empty())
    {
      last_evaluated_type = "array";
      return;
    }

    expr->elements[0]->accept(this);
    std::string first_elem_type = last_evaluated_type;

    for (size_t i = 1; i < expr->elements.size(); i++)
    {
      expr->elements[i]->accept(this);
      if (last_evaluated_type != first_elem_type)
      {
        std::cerr << "Type Error: Array elements must be of homogeneous type." << std::endl;
        exit(1);
      }
    }
    last_evaluated_type = first_elem_type + "[]";
  }

  void visit(ARRAY_ACCESS_EXPRESSION *expr) override
  {
    expr->array_expression->accept(this);
    std::string arr_type = last_evaluated_type;
    expr->index_expression->accept(this);
    if (last_evaluated_type != "int")
    {
      std::cerr << "Type Error: Array index must be 'int'." << std::endl;
      exit(1);
    }
    if (arr_type.length() < 3 || arr_type.substr(arr_type.length() - 2) != "[]")
    {
      std::cerr << "Type Error: Not an array type." << std::endl;
      exit(1);
    }
    last_evaluated_type = arr_type.substr(0, arr_type.length() - 2);
  }

  void visit(CALL_EXPRESSION *expr) override
  {
    if (auto v = dynamic_cast<VARIABLE_EXPRESSION *>(expr->callee))
    {
      if (function_signatures.count(v->name.VALUE))
        last_evaluated_type = function_signatures[v->name.VALUE];
      else
      {
        std::cerr << "Semantic Error: Undefined function '" << v->name.VALUE << "'." << std::endl;
        exit(1);
      }
    }
  }

  void visit(EXPRESSION_STATEMENT *statement) override { statement->expression->accept(this); }
  void visit(PRINT_STATEMENT *statement) override { statement->expression->accept(this); }
  void visit(LOGICAL_EXPRESSION *expr) override { last_evaluated_type = "bool"; }
  void visit(UNARY_EXPRESSION *expr) override { expr->right_operand->accept(this); }

  // [MODIFIED] Return "dynamic_input" to signal the type checker to allow this to be assigned to anything.
  void visit(INPUT_EXPRESSION *expr) override { last_evaluated_type = "dynamic_input"; }
};

#endif