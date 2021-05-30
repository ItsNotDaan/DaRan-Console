#ifndef _SCHERM_H
#define _SCHERM_H

#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

#define Num_Of_Word 4

class scherm
{
  public:
    scherm(void);
    void Animatie_scherm(int A, long tijdTimer);
    void Clear_Display();
    void Scan_Line( unsigned char m);
    void Display(const unsigned char dat[][32]);
    void Send( unsigned char dat);
    void animatie(int A);
private:
};

#endif // _SCHERM_H  //
