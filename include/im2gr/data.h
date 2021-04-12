#ifndef _DATA_H_
#define _DATA_H_

#define MAX_X 600
#define MAX_Y 600
#define MAX_Z 100

class Data {
public:
  Data(std::string f);
  Data(uint8_t ***data, size_t *sz);
  ~Data();

  uint8_t ***get_data();
  size_t    *get_size();
private:
  uint8_t ***data;
  size_t    *sz;
};

#endif /* _DATA_H_ */
