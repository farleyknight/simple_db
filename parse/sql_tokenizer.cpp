
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
