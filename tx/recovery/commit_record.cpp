
class CommitRecord {
public:
  int transaction;

  CommitRecord(int num) {
    transaction = num;
  }

  CommitRecord(LogRecord record) {
    transaction = record.next_int();
  }

  int write_to_log() {
    vector<LogObject> record = {LogObject.from(COMMIT), LogObject.from(transaction)};
  }

  int op() {
    return COMMIT;
  }

  void undo(int num) {
    // NOOP
  }

  int to_string() {
    return "<COMMIT " + transaction + ">";
  }
}
