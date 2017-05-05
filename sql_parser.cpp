#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <exception>
#include <set>
#include <assert.h>

using namespace std;

#include "parse/exceptions.cpp"
#include "parse/sql_tokenizer.cpp"
#include "parse/sql_ast_structs.cpp"
#include "parse/sql_lexer.cpp"
#include "parse/sql_parser.cpp"

void print_vector(vector<string> v) {
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << endl;
  }
}

int main(int, char**)
{
    string text = "SELECT first_name, last_name FROM students WHERE grade = 'A' AND dept = 'History'";
    
    try {
      SQLParser p(text);
      SQLSelectQuery q = p.select();

      // Fields
      assert(q.fields[0] == "first_name");
      assert(q.fields[1] == "last_name");

      // Tables
      assert(q.tables[0] == "students");

      // Predicate
      assert(q.pred.terms[0].left.expr == "grade");
      assert(q.pred.terms[0].right.expr == "'A'");
      assert(q.pred.terms[1].left.expr == "dept");
      assert(q.pred.terms[1].right.expr == "'History'");
      
    } catch (exception& e) {
      cout << e.what() << endl;
      return 1;
    }

    return 0;
}
