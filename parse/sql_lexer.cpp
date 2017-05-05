
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
