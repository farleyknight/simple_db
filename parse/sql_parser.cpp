
class SQLParser {
public:
  SQLLexer lexer;  
  SQLParser(string cmd) : lexer(cmd) { }

  // Parses a select statement
  SQLSelectQuery select() {  
    lexer.next_keyword("SELECT");
    vector<string> fields = field_list();
    lexer.next_keyword("FROM");
    vector<string> tables = table_list();

    SQLPredicate pred; // empty predicate
    if (lexer.matches_keyword("WHERE")) {
      lexer.next_keyword("WHERE");
      pred = predicate(); // actual predicate
    }

    struct SQLSelectQuery statement = {fields, tables, pred};
    return statement;
  }

  string field() {
    return lexer.next_id();
  }
  
  string table() {
    return lexer.next_id();
  }

  SQLConst constant() {
    return lexer.next_const();
  }
  
  vector<string> field_list() {
    vector<string> result;
    result.push_back(field());
    while (lexer.matches_delimiter(",")) {
      lexer.next_token();
      result.push_back(field());
    }
    return result;
  }

  vector<string> table_list() {
    vector<string> result;
    result.push_back(table());
    while (lexer.matches_delimiter(",")) {
      lexer.next_token();
      result.push_back(table());
    }
    return result;
  }

  SQLExpr expression() {
    if (lexer.matches_id()) {
      SQLFieldName f(field());
      return f;
    } else {
      SQLConst c(constant());
      return c;
    }
  }

  SQLTerm term() {
    SQLExpr left = expression();
    lexer.next_delimiter("=");
    SQLExpr right = expression();
    
    return SQLTerm(left, right);
  }

  SQLPredicate predicate() {
    SQLPredicate pred(term());

    while (lexer.matches_keyword("AND")) {
      lexer.next_keyword("AND");
      pred.conjoin(term());
    }
    
    return pred;
  }
};
