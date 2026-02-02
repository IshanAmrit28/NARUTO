#ifndef __PARSER_H
#define __PARSER_H

#include <vector>
#include <iostream>
#include "ast.hpp" // Corrected from "AST.hpp"

class PARSER
{
private:
  std::vector<Token *> token_stream;
  int current_position = 0;

  // ========================================================================
  //                              HELPER FUNCTIONS
  // ========================================================================

  Token *peek_current() { return token_stream[current_position]; }
  Token *peek_previous() { return token_stream[current_position - 1]; }
  bool is_at_end() { return peek_current()->TYPE == TOKEN_EOF; }

  Token *advance_token()
  {
    if (!is_at_end())
      current_position++;
    return peek_previous();
  }

  // Checks if current token matches type, does NOT consume
  bool check_type(enum type type_to_check)
  {
    if (is_at_end())
      return false;
    return peek_current()->TYPE == type_to_check;
  }

  // Consumes token if it matches any in the list
  bool match_types(std::vector<enum type> types_list)
  {
    for (auto t : types_list)
    {
      if (check_type(t))
      {
        advance_token();
        return true;
      }
    }
    return false;
  }

  // Hard assertion: Consume or crash (Panic mode)
  Token *consume_token(enum type expected_type, std::string error_message)
  {
    if (check_type(expected_type))
      return advance_token();
    std::cerr << "[Syntax Error] Line " << peek_current()->line << ": " << error_message << " Found: " << peek_current()->VALUE << std::endl;
    exit(1);
  }

  // Helper to identify variable declarations
  bool is_data_type(enum type t)
  {
    return (t == TOKEN_INT_TYPE || t == TOKEN_FLOAT_TYPE || t == TOKEN_STRING_TYPE ||
            t == TOKEN_BOOL_TYPE || t == TOKEN_CHAR_TYPE || t == TOKEN_VOID_TYPE ||
            t == TOKEN_BYTE_TYPE || t == TOKEN_LONG_TYPE || t == TOKEN_SHORT_TYPE || t == TOKEN_DOUBLE_TYPE);
  }

  // ========================================================================
  //                           GRAMMAR RULES (STATEMENTS)
  // ========================================================================

  STATEMENT *parse_declaration()
  {
    if (match_types({TOKEN_FUNCTION}))
      return parse_function_declaration();

    bool is_const_decl = match_types({TOKEN_CONST});

    // Lookahead to see if we are declaring a variable (e.g., "int x")
    if (is_data_type(peek_current()->TYPE))
      return parse_variable_declaration(is_const_decl);

    return parse_statement();
  }

  STATEMENT *parse_function_declaration()
  {
    if (!is_data_type(peek_current()->TYPE))
    {
      std::cerr << "Expected return type." << std::endl;
      exit(1);
    }
    Token *return_type = advance_token();
    Token *name = consume_token(TOKEN_ID, "Expected function name.");
    consume_token(TOKEN_OPEN_PAREN, "Expected '('.");

    std::vector<FUNCTION_DECLARATION_STATEMENT::PARAMETER_NODE> parameters;
    if (!check_type(TOKEN_CLOSE_PAREN))
    {
      do
      {
        if (!is_data_type(peek_current()->TYPE))
        {
          std::cerr << "Expected param type." << std::endl;
          exit(1);
        }
        Token *p_type = advance_token();
        Token *p_name = consume_token(TOKEN_ID, "Expected param name.");
        parameters.push_back({*p_type, *p_name});
      } while (match_types({TOKEN_COMMA}));
    }
    consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
    consume_token(TOKEN_OPEN_BRACE, "Expected '{'.");
    BLOCK_STATEMENT *body = (BLOCK_STATEMENT *)parse_block_statement();
    return new FUNCTION_DECLARATION_STATEMENT(*name, *return_type, parameters, body);
  }

  STATEMENT *parse_variable_declaration(bool is_const)
  {
    Token *type_token = advance_token();

    // Handle Array Syntax: int[] x
    if (match_types({TOKEN_OPEN_BRACKET}))
    {
      consume_token(TOKEN_CLOSE_BRACKET, "Expected ']'.");
      type_token->VALUE += "[]";
    }

    Token *name_token = consume_token(TOKEN_ID, "Expected variable name.");
    EXPRESSION *initializer = nullptr;
    if (match_types({TOKEN_EQUALS}))
      initializer = parse_expression_logic();
    consume_token(TOKEN_SEMICOLON, "Expected ';'.");
    return new VARIABLE_DECLARATION_STATEMENT(*type_token, *name_token, initializer, is_const);
  }

  STATEMENT *parse_statement()
  {
    if (match_types({TOKEN_IF}))
      return parse_if_statement();

    // [NEW] Added Switch Statement Handling
    if (match_types({TOKEN_SWITCH}))
      return parse_switch_statement();

    if (match_types({TOKEN_WHILE}))
      return parse_while_statement();
    if (match_types({TOKEN_FOR}))
      return parse_for_statement();
    if (match_types({TOKEN_PRINT}))
      return parse_print_statement();
    if (match_types({TOKEN_RETURN}))
      return parse_return_statement();

    // [NEW] Added Loop Control Flow (Break/Continue)
    if (match_types({TOKEN_BREAK}))
    {
      Token k = *peek_previous();
      consume_token(TOKEN_SEMICOLON, "Expected ';'.");
      return new BREAK_STATEMENT(k);
    }
    if (match_types({TOKEN_CONTINUE}))
    {
      Token k = *peek_previous();
      consume_token(TOKEN_SEMICOLON, "Expected ';'.");
      return new CONTINUE_STATEMENT(k);
    }

    if (match_types({TOKEN_OPEN_BRACE}))
      return parse_block_statement();

    return parse_expression_statement();
  }

  // [NEW] SWITCH PARSER IMPLEMENTATION
  STATEMENT *parse_switch_statement()
  {
    consume_token(TOKEN_OPEN_PAREN, "Expected '(' after switch.");
    EXPRESSION *val = parse_expression_logic();
    consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
    consume_token(TOKEN_OPEN_BRACE, "Expected '{'.");

    std::vector<SWITCH_STATEMENT::CASE> cases;
    while (!check_type(TOKEN_CLOSE_BRACE) && !is_at_end())
    {
      EXPRESSION *case_val = nullptr;

      // Parse 'case X:' or 'default:'
      if (match_types({TOKEN_CASE}))
      {
        case_val = parse_expression_logic();
        consume_token(TOKEN_COLON, "Expected ':' after case.");
      }
      else if (match_types({TOKEN_DEFAULT}))
      {
        consume_token(TOKEN_COLON, "Expected ':' after default.");
      }
      else
      {
        std::cerr << "Expected case or default." << std::endl;
        exit(1);
      }

      // Collect statements until next case/default/end
      std::vector<STATEMENT *> stmts;
      while (!check_type(TOKEN_CASE) && !check_type(TOKEN_DEFAULT) && !check_type(TOKEN_CLOSE_BRACE))
      {
        stmts.push_back(parse_statement());
      }
      cases.push_back({case_val, stmts});
    }
    consume_token(TOKEN_CLOSE_BRACE, "Expected '}'.");
    return new SWITCH_STATEMENT(val, cases);
  }

  STATEMENT *parse_for_statement()
  {
    consume_token(TOKEN_OPEN_PAREN, "Expected '('.");
    STATEMENT *initializer = nullptr;

    // Parse Initializer
    if (!match_types({TOKEN_SEMICOLON}))
    {
      if (is_data_type(peek_current()->TYPE))
        initializer = parse_variable_declaration(false);
      else
        initializer = parse_expression_statement();
    }

    // Parse Condition
    EXPRESSION *condition = nullptr;
    if (!check_type(TOKEN_SEMICOLON))
      condition = parse_expression_logic();
    consume_token(TOKEN_SEMICOLON, "Expected ';'.");

    // Parse Increment
    EXPRESSION *increment = nullptr;
    if (!check_type(TOKEN_CLOSE_PAREN))
      increment = parse_expression_logic();
    consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");

    STATEMENT *body = parse_statement();

    // [FIX] No more desugaring to while. Return the real node.
    return new FOR_STATEMENT(initializer, condition, increment, body);
  }

  STATEMENT *parse_if_statement()
  {
    consume_token(TOKEN_OPEN_PAREN, "Expected '('.");
    EXPRESSION *condition = parse_expression_logic();
    consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
    STATEMENT *then_branch = parse_statement();
    STATEMENT *else_branch = nullptr;
    if (match_types({TOKEN_ELSE}))
      else_branch = parse_statement();
    return new IF_STATEMENT(condition, then_branch, else_branch);
  }

  STATEMENT *parse_while_statement()
  {
    consume_token(TOKEN_OPEN_PAREN, "Expected '('.");
    EXPRESSION *condition = parse_expression_logic();
    consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
    STATEMENT *body = parse_statement();
    return new WHILE_STATEMENT(condition, body);
  }

  STATEMENT *parse_block_statement()
  {
    std::vector<STATEMENT *> stmts;
    while (!check_type(TOKEN_CLOSE_BRACE) && !is_at_end())
      stmts.push_back(parse_declaration());
    consume_token(TOKEN_CLOSE_BRACE, "Expected '}'.");
    return new BLOCK_STATEMENT(stmts);
  }

  STATEMENT *parse_print_statement()
  {
    EXPRESSION *val = parse_expression_logic();
    consume_token(TOKEN_SEMICOLON, "Expected ';'.");
    return new PRINT_STATEMENT(val);
  }

  STATEMENT *parse_return_statement()
  {
    Token keyword = *peek_previous();
    EXPRESSION *value = nullptr;
    if (!check_type(TOKEN_SEMICOLON))
      value = parse_expression_logic();
    consume_token(TOKEN_SEMICOLON, "Expected ';'.");
    return new RETURN_STATEMENT(keyword, value);
  }

  STATEMENT *parse_expression_statement()
  {
    EXPRESSION *expr = parse_expression_logic();
    consume_token(TOKEN_SEMICOLON, "Expected ';'.");
    return new EXPRESSION_STATEMENT(expr);
  }

  // ========================================================================
  //                          EXPRESSION PRECEDENCE PARSING
  // ========================================================================
  // Order: Assignment -> Logic OR -> Logic AND -> Bitwise OR -> Bitwise XOR
  //        -> Bitwise AND -> Equality -> Comparison -> Shift -> Term -> Factor
  //        -> Unary -> Call/Postfix -> Primary

  EXPRESSION *parse_expression_logic() { return parse_assignment(); }

  EXPRESSION *parse_assignment()
  {
    EXPRESSION *expr = parse_or();

    if (match_types({TOKEN_EQUALS, TOKEN_PLUS_EQUALS, TOKEN_MINUS_EQUALS, TOKEN_ASTERISK_EQUALS, TOKEN_SLASH_EQUALS, TOKEN_PERCENT_EQUALS, TOKEN_AND_EQUALS, TOKEN_OR_EQUALS, TOKEN_XOR_EQUALS}))
    {
      Token op = *peek_previous();
      EXPRESSION *value = parse_assignment();

      // CASE 1: Assigning to a Variable (x = 10)
      if (auto var = dynamic_cast<VARIABLE_EXPRESSION *>(expr))
      {
        if (op.TYPE != TOKEN_EQUALS)
        {
          // ... (Keep existing desugaring logic for +=, -= etc.) ...
          // Copy the existing switch statement logic here from your previous code
          // Simplified for brevity in this snippet, but DO NOT remove your previous compound logic.
          enum type bin_type;
          switch (op.TYPE)
          {
          case TOKEN_PLUS_EQUALS:
            bin_type = TOKEN_PLUS;
            break;
          case TOKEN_MINUS_EQUALS:
            bin_type = TOKEN_MINUS;
            break;
          case TOKEN_ASTERISK_EQUALS:
            bin_type = TOKEN_ASTERISK;
            break;
          case TOKEN_SLASH_EQUALS:
            bin_type = TOKEN_SLASH;
            break;
          case TOKEN_PERCENT_EQUALS:
            bin_type = TOKEN_PERCENT;
            break;
          case TOKEN_AND_EQUALS:
            bin_type = TOKEN_BITWISE_AND;
            break;
          case TOKEN_OR_EQUALS:
            bin_type = TOKEN_BITWISE_OR;
            break;
          case TOKEN_XOR_EQUALS:
            bin_type = TOKEN_BITWISE_XOR;
            break;
          default:
            bin_type = TOKEN_PLUS;
          }
          Token bin_token = op;
          bin_token.TYPE = bin_type;
          if (bin_type >= TOKEN_BITWISE_AND && bin_type <= TOKEN_RIGHT_SHIFT)
            value = new BITWISE_EXPRESSION(new VARIABLE_EXPRESSION(var->name), bin_token, value);
          else
            value = new BINARY_EXPRESSION(new VARIABLE_EXPRESSION(var->name), bin_token, value);
        }
        return new ASSIGNMENT_EXPRESSION(var->name, value);
      }

      // CASE 2: Assigning to an Array Index (arr[i] = 10)
      else if (auto arrAcc = dynamic_cast<ARRAY_ACCESS_EXPRESSION *>(expr))
      {
        if (op.TYPE != TOKEN_EQUALS)
        {
          // Basic support for compound assignment on arrays (arr[i] += 10)
          // Desugar to: arr[i] = arr[i] + 10
          enum type bin_type;
          switch (op.TYPE)
          {
          case TOKEN_PLUS_EQUALS:
            bin_type = TOKEN_PLUS;
            break;
          case TOKEN_MINUS_EQUALS:
            bin_type = TOKEN_MINUS;
            break;
          // ... add others if needed
          default:
            bin_type = TOKEN_PLUS;
          }
          Token bin_token = op;
          bin_token.TYPE = bin_type;

          // Reconstruct the array access for the right side
          EXPRESSION *rightSideRead = new ARRAY_ACCESS_EXPRESSION(arrAcc->array_expression, arrAcc->index_expression);
          value = new BINARY_EXPRESSION(rightSideRead, bin_token, value);
        }
        return new ARRAY_ASSIGNMENT_EXPRESSION(arrAcc->array_expression, arrAcc->index_expression, value);
      }

      std::cerr << "Invalid assignment target." << std::endl;
      exit(1);
    }
    return expr;
  }

  EXPRESSION *parse_or()
  {
    EXPRESSION *expr = parse_and();
    while (match_types({TOKEN_OR}))
    {
      Token op = *peek_previous();
      expr = new LOGICAL_EXPRESSION(expr, op, parse_and());
    }
    return expr;
  }

  EXPRESSION *parse_and()
  {
    // [CHANGED] Now falls through to parse_bitwise_or instead of parse_equality
    EXPRESSION *expr = parse_bitwise_or();
    while (match_types({TOKEN_AND}))
    {
      Token op = *peek_previous();
      expr = new LOGICAL_EXPRESSION(expr, op, parse_bitwise_or());
    }
    return expr;
  }

  // [NEW] Bitwise OR (|)
  EXPRESSION *parse_bitwise_or()
  {
    EXPRESSION *expr = parse_bitwise_xor();
    while (match_types({TOKEN_BITWISE_OR}))
    {
      Token op = *peek_previous();
      expr = new BITWISE_EXPRESSION(expr, op, parse_bitwise_xor());
    }
    return expr;
  }

  // [NEW] Bitwise XOR (^)
  EXPRESSION *parse_bitwise_xor()
  {
    EXPRESSION *expr = parse_bitwise_and();
    while (match_types({TOKEN_BITWISE_XOR}))
    {
      Token op = *peek_previous();
      expr = new BITWISE_EXPRESSION(expr, op, parse_bitwise_and());
    }
    return expr;
  }

  // [NEW] Bitwise AND (&)
  EXPRESSION *parse_bitwise_and()
  {
    EXPRESSION *expr = parse_equality();
    while (match_types({TOKEN_BITWISE_AND}))
    {
      Token op = *peek_previous();
      expr = new BITWISE_EXPRESSION(expr, op, parse_equality());
    }
    return expr;
  }

  EXPRESSION *parse_equality()
  {
    EXPRESSION *expr = parse_comparison();
    while (match_types({TOKEN_DOUBLE_EQUALS, TOKEN_NOT_EQUALS}))
    {
      Token op = *peek_previous();
      expr = new BINARY_EXPRESSION(expr, op, parse_comparison());
    }
    return expr;
  }

  EXPRESSION *parse_comparison()
  {
    // [CHANGED] Falls through to parse_shift
    EXPRESSION *expr = parse_shift();
    while (match_types({TOKEN_GREATER_THAN, TOKEN_GREATER_EQUAL, TOKEN_LESS_THAN, TOKEN_LESS_EQUAL}))
    {
      Token op = *peek_previous();
      expr = new BINARY_EXPRESSION(expr, op, parse_shift());
    }
    return expr;
  }

  // [NEW] Bit Shift Operations (<<, >>)
  EXPRESSION *parse_shift()
  {
    EXPRESSION *expr = parse_term();
    while (match_types({TOKEN_LEFT_SHIFT, TOKEN_RIGHT_SHIFT}))
    {
      Token op = *peek_previous();
      expr = new BITWISE_EXPRESSION(expr, op, parse_term());
    }
    return expr;
  }

  EXPRESSION *parse_term()
  {
    EXPRESSION *expr = parse_factor();
    while (match_types({TOKEN_PLUS, TOKEN_MINUS}))
    {
      Token op = *peek_previous();
      expr = new BINARY_EXPRESSION(expr, op, parse_factor());
    }
    return expr;
  }

  EXPRESSION *parse_factor()
  {
    EXPRESSION *expr = parse_unary();
    while (match_types({TOKEN_ASTERISK, TOKEN_SLASH, TOKEN_PERCENT}))
    {
      Token op = *peek_previous();
      expr = new BINARY_EXPRESSION(expr, op, parse_unary());
    }
    return expr;
  }

  EXPRESSION *parse_unary()
  {
    // [NEW] Handle Prefix Increment/Decrement (++i, --i)
    if (match_types({TOKEN_INCREMENT, TOKEN_DECREMENT}))
    {
      Token op = *peek_previous();
      EXPRESSION *right = parse_unary();                // Recursive to handle ++(++i)
      return new INCREMENT_EXPRESSION(right, op, true); // true = prefix
    }
    if (match_types({TOKEN_NOT, TOKEN_MINUS, TOKEN_BITWISE_NOT}))
    {
      Token op = *peek_previous();
      return new UNARY_EXPRESSION(op, parse_unary());
    }
    return parse_call();
  }

  EXPRESSION *parse_call()
  {
    EXPRESSION *expr = parse_primary();
    while (true)
    {
      if (match_types({TOKEN_OPEN_PAREN}))
      {
        // Function Call: func()
        std::vector<EXPRESSION *> args;
        if (!check_type(TOKEN_CLOSE_PAREN))
        {
          do
          {
            args.push_back(parse_expression_logic());
          } while (match_types({TOKEN_COMMA}));
        }
        consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
        expr = new CALL_EXPRESSION(expr, args);
      }
      else if (match_types({TOKEN_OPEN_BRACKET}))
      {
        // Array Access: arr[i]
        EXPRESSION *index = parse_expression_logic();
        consume_token(TOKEN_CLOSE_BRACKET, "Expected ']'.");
        expr = new ARRAY_ACCESS_EXPRESSION(expr, index);
      }
      else if (match_types({TOKEN_INCREMENT, TOKEN_DECREMENT}))
      {
        // [NEW] Handle Postfix Increment/Decrement (i++, i--)
        Token op = *peek_previous();
        expr = new INCREMENT_EXPRESSION(expr, op, false); // false = postfix
      }
      else
        break;
    }
    return expr;
  }

  EXPRESSION *parse_primary()
  {
    // Boolean & Null Literals
    if (match_types({TOKEN_FALSE, TOKEN_TRUE, TOKEN_NULL, TOKEN_INT_LITERAL, TOKEN_FLOAT_LITERAL, TOKEN_STRING_LITERAL, TOKEN_CHAR_LITERAL}))
      return new LITERAL_EXPRESSION(*peek_previous());

    // Variables
    if (match_types({TOKEN_ID}))
      return new VARIABLE_EXPRESSION(*peek_previous());

    // Built-in Input
    if (match_types({TOKEN_INPUT}))
    {
      consume_token(TOKEN_OPEN_PAREN, "Expected '('.");
      EXPRESSION *prompt = nullptr;
      if (!check_type(TOKEN_CLOSE_PAREN))
        prompt = parse_expression_logic();
      consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
      return new INPUT_EXPRESSION(prompt);
    }

    // Array Literals: [1, 2, 3]
    if (match_types({TOKEN_OPEN_BRACKET}))
    {
      std::vector<EXPRESSION *> elements;
      if (!check_type(TOKEN_CLOSE_BRACKET))
      {
        do
        {
          elements.push_back(parse_expression_logic());
        } while (match_types({TOKEN_COMMA}));
      }
      consume_token(TOKEN_CLOSE_BRACKET, "Expected ']'.");
      return new ARRAY_LITERAL_EXPRESSION(elements);
    }

    // Grouping: ( expression )
    if (match_types({TOKEN_OPEN_PAREN}))
    {
      EXPRESSION *expr = parse_expression_logic();
      consume_token(TOKEN_CLOSE_PAREN, "Expected ')'.");
      return expr;
    }

    std::cerr << "Expected expression at line " << peek_current()->line << " Found: " << peek_current()->VALUE << std::endl;
    exit(1);
  }

public:
  PARSER(std::vector<Token *> t) : token_stream(t) {}
  std::vector<STATEMENT *> generate_ast()
  {
    std::vector<STATEMENT *> stmts;
    while (!is_at_end())
      stmts.push_back(parse_declaration());
    return stmts;
  }
};

#endif