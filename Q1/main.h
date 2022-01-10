#define STATUS _IOW('a', 'a', struct Status *)

struct Status {
  int size;
 char buff[100];
};
