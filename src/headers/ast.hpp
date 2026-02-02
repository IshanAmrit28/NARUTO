#ifndef __AST_H
#define __AST_H

#include <string>
#include <vector>
#include "tokens.hpp" // Corrected

class AST_VISITOR;

// --- BASE CLASSES ---
class AST_NODE
{
public:
  virtual ~AST_NODE() = default;
  virtual void accept(AST_VISITOR *visitor) = 0;
};

class EXPRESSION : public AST_NODE
{
};
class STATEMENT : public AST_NODE
{
};

// ==========================================
//          EXPRESSION NODES
// ==========================================

class LITERAL_EXPRESSION : public EXPRESSION
{
public:
  Token token;
  LITERAL_EXPRESSION(Token t) : token(t) {}
  void accept(AST_VISITOR *visitor) override;
};

class VARIABLE_EXPRESSION : public EXPRESSION
{
public:
  Token name;
  VARIABLE_EXPRESSION(Token n) : name(n) {}
  void accept(AST_VISITOR *visitor) override;
};

// Binary Math: +, -, *, /, %, <, >, ==
class BINARY_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *left_operand;
  Token operator_token;
  EXPRESSION *right_operand;
  BINARY_EXPRESSION(EXPRESSION *l, Token op, EXPRESSION *r)
      : left_operand(l), operator_token(op), right_operand(r) {}
  void accept(AST_VISITOR *visitor) override;
};

// Bitwise Logic: &, |, ^, <<, >>
class BITWISE_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *left_operand;
  Token operator_token;
  EXPRESSION *right_operand;
  BITWISE_EXPRESSION(EXPRESSION *l, Token op, EXPRESSION *r)
      : left_operand(l), operator_token(op), right_operand(r) {}
  void accept(AST_VISITOR *visitor) override;
};

// Logical: &&, ||
class LOGICAL_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *left_operand;
  Token operator_token;
  EXPRESSION *right_operand;
  LOGICAL_EXPRESSION(EXPRESSION *l, Token op, EXPRESSION *r)
      : left_operand(l), operator_token(op), right_operand(r) {}
  void accept(AST_VISITOR *visitor) override;
};

// Unary: -x, !x, ~x
class UNARY_EXPRESSION : public EXPRESSION
{
public:
  Token operator_token;
  EXPRESSION *right_operand;
  UNARY_EXPRESSION(Token op, EXPRESSION *r) : operator_token(op), right_operand(r) {}
  void accept(AST_VISITOR *visitor) override;
};

// Increment/Decrement: ++i, i++
class INCREMENT_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *variable;
  Token operator_token;
  bool is_prefix; // true = ++i, false = i++
  INCREMENT_EXPRESSION(EXPRESSION *v, Token op, bool prefix)
      : variable(v), operator_token(op), is_prefix(prefix) {}
  void accept(AST_VISITOR *visitor) override;
};

class CALL_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *callee;
  std::vector<EXPRESSION *> arguments;
  CALL_EXPRESSION(EXPRESSION *c, std::vector<EXPRESSION *> args) : callee(c), arguments(args) {}
  void accept(AST_VISITOR *visitor) override;
};

class INPUT_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *prompt_expression;
  INPUT_EXPRESSION(EXPRESSION *p) : prompt_expression(p) {}
  void accept(AST_VISITOR *visitor) override;
};

class ARRAY_LITERAL_EXPRESSION : public EXPRESSION
{
public:
  std::vector<EXPRESSION *> elements;
  ARRAY_LITERAL_EXPRESSION(std::vector<EXPRESSION *> e) : elements(e) {}
  void accept(AST_VISITOR *visitor) override;
};

class ARRAY_ACCESS_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *array_expression;
  EXPRESSION *index_expression;
  ARRAY_ACCESS_EXPRESSION(EXPRESSION *arr, EXPRESSION *idx) : array_expression(arr), index_expression(idx) {}
  void accept(AST_VISITOR *visitor) override;
};

class ARRAY_ASSIGNMENT_EXPRESSION : public EXPRESSION
{
public:
  EXPRESSION *array_expression;
  EXPRESSION *index_expression;
  EXPRESSION *value_expression;
  ARRAY_ASSIGNMENT_EXPRESSION(EXPRESSION *arr, EXPRESSION *idx, EXPRESSION *val)
      : array_expression(arr), index_expression(idx), value_expression(val) {}
  void accept(AST_VISITOR *visitor) override;
};
class ASSIGNMENT_EXPRESSION : public EXPRESSION
{
public:
  Token variable_name;
  EXPRESSION *value_expression;
  ASSIGNMENT_EXPRESSION(Token n, EXPRESSION *v) : variable_name(n), value_expression(v) {}
  void accept(AST_VISITOR *visitor) override;
};

// ==========================================
//          STATEMENT NODES
// ==========================================

class EXPRESSION_STATEMENT : public STATEMENT
{
public:
  EXPRESSION *expression;
  EXPRESSION_STATEMENT(EXPRESSION *e) : expression(e) {}
  void accept(AST_VISITOR *visitor) override;
};

class PRINT_STATEMENT : public STATEMENT
{
public:
  EXPRESSION *expression;
  PRINT_STATEMENT(EXPRESSION *e) : expression(e) {}
  void accept(AST_VISITOR *visitor) override;
};

class VARIABLE_DECLARATION_STATEMENT : public STATEMENT
{
public:
  Token type_token;
  Token name_token;
  EXPRESSION *initializer_expression;
  bool is_constant;
  VARIABLE_DECLARATION_STATEMENT(Token t, Token n, EXPRESSION *init, bool c)
      : type_token(t), name_token(n), initializer_expression(init), is_constant(c) {}
  void accept(AST_VISITOR *visitor) override;
};

class BLOCK_STATEMENT : public STATEMENT
{
public:
  std::vector<STATEMENT *> statements;
  BLOCK_STATEMENT(std::vector<STATEMENT *> s) : statements(s) {}
  void accept(AST_VISITOR *visitor) override;
};

class IF_STATEMENT : public STATEMENT
{
public:
  EXPRESSION *condition_expression;
  STATEMENT *then_branch_statement;
  STATEMENT *else_branch_statement;
  IF_STATEMENT(EXPRESSION *c, STATEMENT *t, STATEMENT *e) : condition_expression(c), then_branch_statement(t), else_branch_statement(e) {}
  void accept(AST_VISITOR *visitor) override;
};

// Switch Case
class SWITCH_STATEMENT : public STATEMENT
{
public:
  EXPRESSION *value;
  struct CASE
  {
    EXPRESSION *condition; // null for default
    std::vector<STATEMENT *> statements;
  };
  std::vector<CASE> cases;
  SWITCH_STATEMENT(EXPRESSION *v, std::vector<CASE> c) : value(v), cases(c) {}
  void accept(AST_VISITOR *visitor) override;
};

class WHILE_STATEMENT : public STATEMENT
{
public:
  EXPRESSION *condition_expression;
  STATEMENT *body_statement;
  WHILE_STATEMENT(EXPRESSION *c, STATEMENT *b) : condition_expression(c), body_statement(b) {}
  void accept(AST_VISITOR *visitor) override;
};

class FOR_STATEMENT : public STATEMENT
{
public:
  STATEMENT *initializer;
  EXPRESSION *condition;
  EXPRESSION *increment;
  STATEMENT *body;
  FOR_STATEMENT(STATEMENT *i, EXPRESSION *c, EXPRESSION *inc, STATEMENT *b)
      : initializer(i), condition(c), increment(inc), body(b) {}
  void accept(AST_VISITOR *visitor) override;
};

// Loop Controls
class BREAK_STATEMENT : public STATEMENT
{
public:
  Token keyword;
  BREAK_STATEMENT(Token k) : keyword(k) {}
  void accept(AST_VISITOR *visitor) override;
};

class CONTINUE_STATEMENT : public STATEMENT
{
public:
  Token keyword;
  CONTINUE_STATEMENT(Token k) : keyword(k) {}
  void accept(AST_VISITOR *visitor) override;
};

class RETURN_STATEMENT : public STATEMENT
{
public:
  Token keyword_token;
  EXPRESSION *value_expression;
  RETURN_STATEMENT(Token k, EXPRESSION *v) : keyword_token(k), value_expression(v) {}
  void accept(AST_VISITOR *visitor) override;
};

class FUNCTION_DECLARATION_STATEMENT : public STATEMENT
{
public:
  Token name_token;
  Token return_type_token;
  struct PARAMETER_NODE
  {
    Token type_token;
    Token name_token;
  };
  std::vector<PARAMETER_NODE> parameters;
  BLOCK_STATEMENT *body_block;
  FUNCTION_DECLARATION_STATEMENT(Token n, Token r, std::vector<PARAMETER_NODE> p, BLOCK_STATEMENT *b)
      : name_token(n), return_type_token(r), parameters(p), body_block(b) {}
  void accept(AST_VISITOR *visitor) override;
};

// ==========================================
//          VISITOR INTERFACE
// ==========================================
class AST_VISITOR
{
public:
  virtual void visit(LITERAL_EXPRESSION *expression) = 0;
  virtual void visit(VARIABLE_EXPRESSION *expression) = 0;
  virtual void visit(BINARY_EXPRESSION *expression) = 0;
  virtual void visit(BITWISE_EXPRESSION *expression) = 0;
  virtual void visit(LOGICAL_EXPRESSION *expression) = 0;
  virtual void visit(UNARY_EXPRESSION *expression) = 0;
  virtual void visit(INCREMENT_EXPRESSION *expression) = 0;
  virtual void visit(CALL_EXPRESSION *expression) = 0;
  virtual void visit(INPUT_EXPRESSION *expression) = 0;
  virtual void visit(ARRAY_LITERAL_EXPRESSION *expression) = 0;
  virtual void visit(ARRAY_ACCESS_EXPRESSION *expression) = 0;
  virtual void visit(ARRAY_ASSIGNMENT_EXPRESSION *expression) = 0;
  virtual void visit(ASSIGNMENT_EXPRESSION *expression) = 0;

  virtual void visit(EXPRESSION_STATEMENT *statement) = 0;
  virtual void visit(PRINT_STATEMENT *statement) = 0;
  virtual void visit(VARIABLE_DECLARATION_STATEMENT *statement) = 0;
  virtual void visit(BLOCK_STATEMENT *statement) = 0;
  virtual void visit(IF_STATEMENT *statement) = 0;
  virtual void visit(SWITCH_STATEMENT *statement) = 0;
  virtual void visit(WHILE_STATEMENT *statement) = 0;
  virtual void visit(FOR_STATEMENT *statement) = 0;
  virtual void visit(BREAK_STATEMENT *statement) = 0;
  virtual void visit(CONTINUE_STATEMENT *statement) = 0;
  virtual void visit(RETURN_STATEMENT *statement) = 0;
  virtual void visit(FUNCTION_DECLARATION_STATEMENT *statement) = 0;
};

// Inline Implementations
inline void LITERAL_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void VARIABLE_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void BINARY_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void BITWISE_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void LOGICAL_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void UNARY_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void INCREMENT_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void CALL_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void INPUT_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void ARRAY_LITERAL_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void ARRAY_ACCESS_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void ARRAY_ASSIGNMENT_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void ASSIGNMENT_EXPRESSION::accept(AST_VISITOR *v) { v->visit(this); }
inline void EXPRESSION_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void PRINT_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void VARIABLE_DECLARATION_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void BLOCK_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void IF_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void SWITCH_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void WHILE_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void FOR_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void BREAK_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void CONTINUE_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void RETURN_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }
inline void FUNCTION_DECLARATION_STATEMENT::accept(AST_VISITOR *v) { v->visit(this); }

#endif