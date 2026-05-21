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
  struct ClassTypeInfo
  {
    std::string name;
    std::string superclass;
    bool is_struct = false;
    std::unordered_map<std::string, std::string> field_types;
    std::unordered_map<std::string, bool> is_private;
    std::unordered_map<std::string, std::string> method_return_types;
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> method_params;
    std::vector<std::pair<std::string, std::string>> struct_fields;
  };
  std::unordered_map<std::string, ClassTypeInfo> class_registry;
  std::string current_class = "";

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

    // Check inheritance chain for polymorphism
    std::string curr = source;
    while (class_registry.count(curr))
    {
      curr = class_registry[curr].superclass;
      if (curr == target)
        return true;
    }

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
      return true; // Python-like implicit numeric conversion
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
  void visit(FOR_STATEMENT *stmt) override
  {
    enter_new_scope(); // For loops have their own scope (for int i=0...)

    if (stmt->initializer)
      stmt->initializer->accept(this);

    if (stmt->condition)
    {
      stmt->condition->accept(this);
      if (last_evaluated_type != "bool")
      {
        std::cerr << "Type Error: For loop condition must be bool." << std::endl;
        exit(1);
      }
    }

    if (stmt->increment)
      stmt->increment->accept(this);

    loop_depth++;
    stmt->body->accept(this);
    loop_depth--;

    exit_current_scope();
  }

  void visit(FUNCTION_DECLARATION_STATEMENT *statement) override
  {
    enter_new_scope();
    current_function_return_type = statement->return_type_token.VALUE;
    for (auto p : statement->parameters)
      declare_variable(p.name_token.VALUE, p.type_token.VALUE);
    
    int previous_loop_depth = loop_depth;
    loop_depth = 0;
    
    statement->body_block->accept(this);
    
    loop_depth = previous_loop_depth;
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
      std::string name = v->name.VALUE;
      
      if (name == "int" || name == "float" || name == "string")
      {
        if (expr->arguments.size() != 1)
        {
          std::cerr << "Semantic Error: '" << name << "' conversion expects exactly 1 argument." << std::endl;
          exit(1);
        }
        expr->arguments[0]->accept(this);
        last_evaluated_type = name;
        return;
      }

      if (class_registry.count(name))
      {
        ClassTypeInfo info = class_registry[name];
        if (info.is_struct)
        {
          // Struct positional constructor call
          if (expr->arguments.size() != info.struct_fields.size())
          {
            std::cerr << "Semantic Error: Struct '" << name << "' expects "
                      << info.struct_fields.size() << " fields, got " << expr->arguments.size() << "." << std::endl;
            exit(1);
          }
          for (size_t i = 0; i < expr->arguments.size(); i++)
          {
            expr->arguments[i]->accept(this);
            std::string arg_type = last_evaluated_type;
            std::string expected_type = info.struct_fields[i].second;
            if (!can_assign(expected_type, arg_type))
            {
              std::cerr << "Type Error: Struct '" << name << "' field '"
                        << info.struct_fields[i].first << "' expects '" << expected_type << "', got '" << arg_type << "'." << std::endl;
              exit(1);
            }
          }
          last_evaluated_type = name;
          return;
        }
      }

      if (function_signatures.count(name))
      {
        last_evaluated_type = function_signatures[name];
        return;
      }

      std::cerr << "Semantic Error: Undefined function or struct constructor '" << name << "'." << std::endl;
      exit(1);
    }
    else if (auto get_expr = dynamic_cast<GET_EXPRESSION *>(expr->callee))
    {
      get_expr->object_expression->accept(this);
      std::string obj_type = last_evaluated_type;
      
      if (obj_type.substr(0, 11) == "super_type:") {
          obj_type = obj_type.substr(11);
      }

      if (obj_type.substr(0, 11) == "super_type:") {
          obj_type = obj_type.substr(11);
      }

      if (obj_type.substr(obj_type.length() >= 2 ? obj_type.length() - 2 : 0) == "[]") {
          std::string method_name = get_expr->member_name.VALUE;
          if (method_name == "push") {
              if (expr->arguments.size() != 1) {
                  std::cerr << "Semantic Error: push() expects exactly 1 argument." << std::endl;
                  exit(1);
              }
              expr->arguments[0]->accept(this);
              std::string arg_type = last_evaluated_type;
              std::string elem_type = obj_type.substr(0, obj_type.length() - 2);
              if (!can_assign(elem_type, arg_type)) {
                  std::cerr << "Type Error: Cannot push type '" << arg_type << "' into array of '" << elem_type << "'." << std::endl;
                  exit(1);
              }
              last_evaluated_type = "void";
              return;
          }
      }

      if (!class_registry.count(obj_type))
      {
        std::cerr << "Type Error: Type '" << obj_type << "' has no members." << std::endl;
        exit(1);
      }

      ClassTypeInfo info = class_registry[obj_type];
      std::string method_name = get_expr->member_name.VALUE;

      if (!info.method_return_types.count(method_name))
      {
        std::cerr << "Semantic Error: Method '" << method_name << "' not found on type '" << obj_type << "'." << std::endl;
        exit(1);
      }

      if (info.is_private[method_name])
      {
        if (current_class != obj_type)
        {
          std::cerr << "Semantic Error: Member '" << method_name << "' of class '"
                    << obj_type << "' is private and can only be accessed within the class." << std::endl;
          exit(1);
        }
      }

      auto expected_params = info.method_params[method_name];
      if (expr->arguments.size() != expected_params.size())
      {
        std::cerr << "Semantic Error: Method '" << method_name << "' expects "
                  << expected_params.size() << " arguments, got " << expr->arguments.size() << "." << std::endl;
        exit(1);
      }

      for (size_t i = 0; i < expr->arguments.size(); i++)
      {
        expr->arguments[i]->accept(this);
        std::string arg_type = last_evaluated_type;
        std::string expected_type = expected_params[i].first;
        if (!can_assign(expected_type, arg_type))
        {
          std::cerr << "Type Error: Method '" << method_name << "' parameter " << (i + 1)
                    << " expects '" << expected_type << "', got '" << arg_type << "'." << std::endl;
          exit(1);
        }
      }

      last_evaluated_type = info.method_return_types[method_name];
    }
    else if (auto super_expr = dynamic_cast<SUPER_EXPRESSION *>(expr->callee))
    {
        super_expr->accept(this);
        std::string obj_type = last_evaluated_type;
        if (obj_type.substr(0, 11) == "super_type:") {
            obj_type = obj_type.substr(11);
        }
        
        ClassTypeInfo info = class_registry[obj_type];
        if (info.method_return_types.count("init"))
        {
          auto expected_params = info.method_params["init"];
          if (expr->arguments.size() != expected_params.size())
          {
            std::cerr << "Semantic Error: Constructor 'init' for class '" << obj_type
                      << "' expects " << expected_params.size() << " arguments, got "
                      << expr->arguments.size() << "." << std::endl;
            exit(1);
          }
          for (size_t i = 0; i < expr->arguments.size(); i++)
          {
            expr->arguments[i]->accept(this);
            std::string arg_type = last_evaluated_type;
            std::string expected_type = expected_params[i].first;
            if (!can_assign(expected_type, arg_type))
            {
              std::cerr << "Type Error: Method 'init' parameter " << (i + 1)
                        << " expects '" << expected_type << "', got '" << arg_type << "'." << std::endl;
              exit(1);
            }
          }
        }
        else if (!expr->arguments.empty())
        {
          std::cerr << "Semantic Error: Class '" << obj_type << "' does not define an 'init' constructor, but arguments were provided." << std::endl;
          exit(1);
        }
        last_evaluated_type = "void";
    }
    else
    {
      expr->callee->accept(this);
    }
  }

  void visit(CLASS_DECLARATION_STATEMENT *stmt) override
  {
    ClassTypeInfo info;
    info.name = stmt->name_token.VALUE;
    info.superclass = stmt->superclass_token.VALUE;
    info.is_struct = false;
    info.is_private = stmt->is_private;

    if (!info.superclass.empty())
    {
      if (!class_registry.count(info.superclass))
      {
        std::cerr << "Semantic Error: Superclass '" << info.superclass << "' is undefined." << std::endl;
        exit(1);
      }
      if (class_registry[info.superclass].is_struct)
      {
        std::cerr << "Semantic Error: Class '" << info.name << "' cannot inherit from a struct." << std::endl;
        exit(1);
      }
      info.field_types = class_registry[info.superclass].field_types;
      info.is_private.insert(class_registry[info.superclass].is_private.begin(), class_registry[info.superclass].is_private.end());
      info.method_return_types = class_registry[info.superclass].method_return_types;
      info.method_params = class_registry[info.superclass].method_params;
    }

    for (auto field : stmt->fields)
    {
      info.field_types[field->name_token.VALUE] = field->type_token.VALUE;
    }

    std::string old_class = current_class;
    current_class = info.name;
    for (auto method : stmt->methods)
    {
      info.method_return_types[method->name_token.VALUE] = method->return_type_token.VALUE;
      std::vector<std::pair<std::string, std::string>> params;
      for (auto &p : method->parameters)
      {
        params.push_back({p.type_token.VALUE, p.name_token.VALUE});
      }
      info.method_params[method->name_token.VALUE] = params;
    }

    class_registry[info.name] = info;

    enter_new_scope();
    declare_variable("this", info.name);
    for (auto &f : info.field_types)
    {
      declare_variable(f.first, f.second);
    }
    for (auto method : stmt->methods)
    {
      method->accept(this);
    }
    exit_current_scope();
    current_class = old_class;
  }

  void visit(STRUCT_DECLARATION_STATEMENT *stmt) override
  {
    ClassTypeInfo info;
    info.name = stmt->name_token.VALUE;
    info.is_struct = true;

    for (auto field : stmt->fields)
    {
      info.field_types[field->name_token.VALUE] = field->type_token.VALUE;
      info.is_private[field->name_token.VALUE] = false;
      info.struct_fields.push_back({field->name_token.VALUE, field->type_token.VALUE});
    }

    class_registry[info.name] = info;

    enter_new_scope();
    for (auto field : stmt->fields)
    {
      field->accept(this);
    }
    exit_current_scope();
  }

  void visit(NEW_EXPRESSION *expr) override
  {
    std::string class_name = expr->class_name.VALUE;
    if (!class_registry.count(class_name))
    {
      std::cerr << "Semantic Error: Undefined class '" << class_name << "'." << std::endl;
      exit(1);
    }
    ClassTypeInfo info = class_registry[class_name];
    if (info.is_struct)
    {
      std::cerr << "Semantic Error: Struct '" << class_name << "' cannot be instantiated with 'new'." << std::endl;
      exit(1);
    }

    if (info.method_return_types.count("init"))
    {
      auto expected_params = info.method_params["init"];
      if (expr->arguments.size() != expected_params.size())
      {
        std::cerr << "Semantic Error: Constructor 'init' for class '" << class_name
                  << "' expects " << expected_params.size() << " arguments, got "
                  << expr->arguments.size() << "." << std::endl;
        exit(1);
      }
      for (size_t i = 0; i < expr->arguments.size(); ++i)
      {
        expr->arguments[i]->accept(this);
        std::string arg_type = last_evaluated_type;
        std::string expected_type = expected_params[i].first;
        if (!can_assign(expected_type, arg_type))
        {
          std::cerr << "Type Error: Constructor 'init' parameter " << (i + 1)
                    << " expects type '" << expected_type << "', got '" << arg_type << "'." << std::endl;
          exit(1);
        }
      }
    }
    else
    {
      if (!expr->arguments.empty())
      {
        std::cerr << "Semantic Error: Class '" << class_name
                  << "' does not define an 'init' constructor, but arguments were provided." << std::endl;
        exit(1);
      }
    }

    last_evaluated_type = class_name;
  }

  void visit(GET_EXPRESSION *expr) override
  {
    expr->object_expression->accept(this);
    std::string obj_type = last_evaluated_type;

    if (obj_type.substr(obj_type.length() >= 2 ? obj_type.length() - 2 : 0) == "[]" || obj_type == "string")
    {
      if (expr->member_name.VALUE == "length") {
        last_evaluated_type = "int";
        return;
      }
    }

    if (!class_registry.count(obj_type))
    {
      std::cerr << "Type Error: Type '" << obj_type << "' has no members." << std::endl;
      exit(1);
    }

    ClassTypeInfo info = class_registry[obj_type];
    std::string member = expr->member_name.VALUE;

    if (info.is_private[member])
    {
      if (current_class != obj_type)
      {
        std::cerr << "Semantic Error: Member '" << member << "' of class '"
                  << obj_type << "' is private and can only be accessed within the class." << std::endl;
        exit(1);
      }
    }

    if (info.field_types.count(member))
    {
      last_evaluated_type = info.field_types[member];
    }
    else if (info.method_return_types.count(member))
    {
      std::cerr << "Semantic Error: Method '" << member << "' cannot be accessed without invoking it." << std::endl;
      exit(1);
    }
    else
    {
      std::cerr << "Semantic Error: Member '" << member << "' not found on type '" << obj_type << "'." << std::endl;
      exit(1);
    }
  }

  void visit(SET_EXPRESSION *expr) override
  {
    expr->object_expression->accept(this);
    std::string obj_type = last_evaluated_type;

    if (!class_registry.count(obj_type))
    {
      std::cerr << "Type Error: Type '" << obj_type << "' has no members." << std::endl;
      exit(1);
    }

    ClassTypeInfo info = class_registry[obj_type];
    std::string member = expr->member_name.VALUE;

    if (!info.field_types.count(member))
    {
      std::cerr << "Semantic Error: Field '" << member << "' not found on type '" << obj_type << "'." << std::endl;
      exit(1);
    }

    if (info.is_private[member])
    {
      if (current_class != obj_type)
      {
        std::cerr << "Semantic Error: Member '" << member << "' of class '"
                  << obj_type << "' is private and can only be modified within the class." << std::endl;
        exit(1);
      }
    }

    std::string expected_type = info.field_types[member];
    expr->value_expression->accept(this);
    std::string assigned_type = last_evaluated_type;

    if (!can_assign(expected_type, assigned_type))
    {
      std::cerr << "Type Error: Cannot assign '" << assigned_type << "' to field '"
                << member << "' of type '" << expected_type << "'." << std::endl;
      exit(1);
    }

    last_evaluated_type = assigned_type;
  }

  void visit(EXPRESSION_STATEMENT *statement) override { statement->expression->accept(this); }
  void visit(PRINT_STATEMENT *statement) override { statement->expression->accept(this); }
  void visit(LOGICAL_EXPRESSION *expr) override { last_evaluated_type = "bool"; }
  void visit(UNARY_EXPRESSION *expr) override { expr->right_operand->accept(this); }

  // [MODIFIED] Return "dynamic_input" to signal the type checker to allow this to be assigned to anything.
  void visit(INPUT_EXPRESSION *expr) override { last_evaluated_type = "dynamic_input"; }

  void visit(SUPER_EXPRESSION *expr) override
  {
    if (current_class.empty())
    {
      std::cerr << "Semantic Error: Cannot use 'super' outside of a class." << std::endl;
      exit(1);
    }
    std::string superclass = class_registry[current_class].superclass;
    if (superclass.empty())
    {
      std::cerr << "Semantic Error: Class '" << current_class << "' does not have a superclass." << std::endl;
      exit(1);
    }
    last_evaluated_type = "super_type:" + superclass;
  }
};

#endif