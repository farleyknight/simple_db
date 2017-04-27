
class LogManager {
public:
  const static int LAST_POS = 0;
  
  std::string logfile;
  int current_position;
  Block *current_block;
  Page *page;
  FileManager file_manager;
  
  LogManager() : logfile("simpledb.log") {    
    int logfile_size = file_manager.size(logfile);
    if (logfile_size == 0) {
      append_new_block();
    } else {
      current_block = new Block(logfile, logfile_size - 1);
      page->read(*current_block);
      current_position = get_last_record_position() + ByteBuffer::INT_SIZE;
    }
  }

  void append_new_block() {
    set_last_record_position(0);
    current_position = ByteBuffer::INT_SIZE;
    *current_block = page->append(logfile);
  }

  int get_last_record_position() {
    return page->get_int(LAST_POS);
  }

  void set_last_record_position(int pos) {
    page->set_int(LAST_POS, pos);
  }

  void flush(int lsn) {
    if (lsn >= current_lsn())
      flush();
  }

  int append(Object[] rec) {
    int record_size = ByteBuffer::INT_SIZE;
    
    for (Object obj : rec)
      record_size += size(obj);
    
    if (current_position + record_size >= ByteBuffer::BLOCK_SIZE) {
      flush();
      append_new_block();
    }
    for (Object obj : rec)
      append_val(obj);
    finalizeRecord();
      return currentLSN();
  }

   int size(int value) {
     return INT_SIZE;
   }

  int size(std::string value) {
    return ByteBuffer::STR_SIZE(value.size());
  }
  
};
