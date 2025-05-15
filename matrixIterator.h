
typedef struct matrixIterator mIterator;

mIterator * init(const char * filename);

bool next(mIterator * mIt);

void print(mIterator * mIt);
