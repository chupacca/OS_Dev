#define ROW 5
#define COL 5

// MATRIX ROUTINES
int ** AllocMatrix(int r, int c);
void FreeMatrix(int ** a, int r, int c);
void GenMatrix(int ** matrix, const int height, const int width);
void GenMatrixType(int ** matrix, const int height, const int width, int type);
int AvgElement(int ** matrix, const int height, const int width);
int SumMatrix(int ** matrix, const int height, const int width);
void DisplayMatrix(int ** matrix, const int height, const int width, FILE *stream);

