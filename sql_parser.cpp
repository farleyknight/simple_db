#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <exception>
#include <set>

using namespace std;

class SQLTokenizer {
public:
  string input;
  
  SQLTokenizer(string s) : input(s) { }

  queue<string> to_tokens() {
    int index = 0;
    queue<string> tokens;
    
    // TODO: Modify for quoted strings with spaces inside
    while (index < input.size()) {
      string token;
      while (input[index] != ' ' && index < input.size()) {
	if (input[index] == ',') {
	  tokens.push(token);
	  tokens.push(",");
	  token = "";
	} else {
	  token += input[index];
	}
	index++;
      }
      
      if (token != "") {
	tokens.push(token);
      }
      index++;
    }
    return tokens;
  }
};

////////////////
// Exceptions //
////////////////

class WrongKeywordException : public runtime_error {
public:
  WrongKeywordException(string s) : runtime_error(s) { }
};

class WrongSyntaxException : public runtime_error {
public:
  WrongSyntaxException(string s) : runtime_error(s) { }
};

/////////////
// Structs //
/////////////

struct SQLExpr {
  string expr;
  SQLExpr() { }
  SQLExpr(string e) : expr(e) { }
};

void print_expr(SQLExpr e) {
  cout << "expr = " << e.expr << endl;
}

struct SQLFieldName : SQLExpr {
  SQLFieldName(string e) : SQLExpr(e) { }
};

struct SQLConst : SQLExpr {
  SQLConst(string e) : SQLExpr(e) { }
};

struct SQLStringConst : SQLConst {
  SQLStringConst(string e) : SQLConst(e) { }
};

struct SQLIntConst : SQLConst {
  int value;
  SQLIntConst(string e) : SQLConst(e), value(std::atoi(e.c_str())) { }
};

struct SQLTerm {
  SQLExpr left, right;
  SQLTerm() { }
  SQLTerm(SQLExpr l, SQLExpr r) : left(l), right(r) { }
};

struct SQLPredicate {
  vector<SQLTerm> terms;
  SQLPredicate() { }
  SQLPredicate(SQLTerm t) {
    terms.push_back(t);
  }

  void conjoin(SQLPredicate p) {
    for (int i = 0; i < p.terms.size(); i++) {
      terms.push_back(p.terms[i]);
    }
  }
};

struct SQLSelectQuery {
  vector<string> fields;
  vector<string> tables;
  SQLPredicate pred;
};


////////////////////
// Lexer & Parser //
////////////////////

class SQLLexer {
public:
  queue<string> tokens;
  string input;
  set<string> keywords;
  
  SQLLexer(string s) : input(s) {
    SQLTokenizer w(s);
    tokens = w.to_tokens();
    keywords = {
      "SELECT", "INSERT", "UPDATE", "DELETE", "AND", "OR"
    };
  }
  
  string next_token() {
    string s = tokens.front();
    tokens.pop();
    return s;
  }

  string next_delimiter(string d) {
    if (!matches_delimiter(d)) {
      string message = "Error in query: Expected delimiter '" + d + 
	"' but got '" + tokens.front() + "'";
      throw WrongSyntaxException(message);
    }
    return next_token();
  }

  bool matches_keyword(string keyword) {
    return tokens.front() == keyword;
  }
  
  string next_keyword(string keyword) {
    if (!matches_keyword(keyword)) {
      string message = "Error in query: Expected keyword '"
	+ keyword + "' but got '" + tokens.front() + "'";
      throw WrongKeywordException(message);
    }
    return next_token();
  }

  bool is_id(string s) {
    for (int i = 0; i < s.size(); i++) {
      char c = s[i];
      if (!isalpha(c) && c != '_')
	return false;
    }
    return true;
  }

  bool is_number(string s) {
    for (int i = 0; i < s.size(); i++) {
      char c = s[i];
      if (!isdigit(c))
	return false;
    }
    return true;
  }

  bool matches_id() {
    return is_id(tokens.front()) && keywords.count(tokens.front()) == 0;
  }

  string next_id() {
    if (!matches_id()) {
      string message = "Error in query: Expected identifier but got '" + tokens.front() + "'";
      throw WrongSyntaxException(message);
    }
    return next_token();
  }

  bool matches_delimiter(string s) {
    return tokens.front() == s;
  }

  // TODO: Do we handle spaces in quoted strings, in the tokenizer?
  // Probably not!!!
  bool matches_string_constant() {
    string s = tokens.front();
    return (s[0] == '\'' && s[s.size() - 1] == '\'');
  }

  bool matches_int_constant() {
    string s = tokens.front();
    return is_number(s);
  }

  SQLConst next_const() {
    if (matches_string_constant()) {
      SQLStringConst s(next_token());
      return s;
    } else if (matches_int_constant()) {
      SQLIntConst i(next_token());
      return i;
    } else {
      string message = "Error in query: Expected string or integer constant but got '"
	+ tokens.front() + "'";
      throw WrongSyntaxException(message);      
    }
  }
};


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

void print_vector(vector<string> v) {
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << endl;
  }
}

void print_predicate(SQLPredicate pred) {
  for (int i = 0; i < pred.terms.size(); i++) {
    cout << "left = " << pred.terms[i].left.expr << endl;
    cout << "right = " << pred.terms[i].right.expr << endl;
  }
}

int main(int, char**)
{
    string text = "SELECT first_name, last_name FROM students WHERE grade = 'A' AND dept = 'History'";
    
    try {
      SQLParser p(text);
      SQLSelectQuery q = p.select();

      cout << "got fields:" << endl;
      print_vector(q.fields);
      cout << endl;
      
      cout << "got tables:" << endl;
      print_vector(q.tables);
      cout << endl;

      cout << "got pred:" << endl;
      print_predicate(q.pred);
      
    } catch (exception& e) {
      cout << e.what() << endl;
      return 1;
    }

    return 0;
}
