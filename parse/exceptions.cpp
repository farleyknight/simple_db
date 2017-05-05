
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
