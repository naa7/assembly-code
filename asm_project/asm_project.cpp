#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <fstream>

using namespace std;


bool eight_bit_search(unsigned char byte_array[], int pos) {
    unsigned short eight_bit[] = { 0x00,0x02,0x04,0x08, 0x0A,0x0C,0x10, 0x12, 0x14, 0x18,0x20, 0x22,0x24,0x1A,0x1C,0x28,0x2A,0x2C };
    bool  true_false = false;
    for (int i = 0; i < sizeof(eight_bit); i++) {

        if (eight_bit[i] == byte_array[pos])
            true_false = true;
    }
    return true_false;
}

bool sixteen_bit_search(unsigned char byte_array[], int pos) {
      unsigned short sixteen_bit[] = { 0x01,0x03,0x05,0x06,0x07,0x09,0x0B,0x0D,0x0E,0x11,0x13,0x15,0x19,0x1B,0x1D,0x1E,0x1F,0x21,0x23,0x25,
                                       0x29,0x2B,0x2D,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
                                       0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F };
      bool true_false = false;
      for (int i = 0; i < sizeof(sixteen_bit); i++) {

          if (sixteen_bit[i] == byte_array[pos])
          true_false = true;
      }
      return true_false;
}

int check_if_zero(unsigned char byte_array[], int pos) {

    if (byte_array[pos] == 0)
        return 1;
    else
        return 0;
}

int check_if_carry_or_overflow(unsigned char byte_array[], int pos) {

    int size = *(&byte_array + 1) - byte_array;
    int carry = 0;
    for (int i = 0; i < 8; i++) {
        if (byte_array[i] > 256)
            carry = 1;
    }
    return carry;
}

int check_byte_sign(unsigned char byte_array[], int pos) {

    if (byte_array[pos] < 0)
        return 1;
    if (byte_array[pos] >= 0)
        return 0;
}

int Flags_1() {

    unsigned short IP = 0x100;
    unsigned char buffer[100000];
    unsigned short FLAGS[3] = {0,0,0};
    FLAGS[0] = check_if_zero(buffer, IP);
    FLAGS[1] = check_if_carry_or_overflow(buffer, IP);
    FLAGS[2] = check_byte_sign(buffer, IP);
    return (FLAGS[0],FLAGS[1],FLAGS[2]);
}

int Flags_2() {

    unsigned short IP = 0x100;
    unsigned char buffer[100000];
    unsigned short FLAGS[3] = {0,0,0};
    FLAGS[0] = check_if_zero(buffer, IP);
    FLAGS[1] = 0;
    FLAGS[2] = check_byte_sign(buffer, IP);
    return (FLAGS[0],FLAGS[1],FLAGS[2]);
}

int main() {

    int size = 8;
    bool secure = false;
    int start = 0x100;
    unsigned char buffer[100000];
    unsigned short regs[4] = { 0,0,0,0 };
    unsigned short point_reg[4] = { 0x100,0x100,0x100,0x100 };
    unsigned short FLAGS[3] = {0,0,0};
    unsigned short IP = 0x100;
    FILE* file_open;
    file_open = fopen("SAMPLE.COM", "rb");

    if (!file_open) {
        cout << "Error, file could not open" << endl;
        return 1;
      }
    unsigned short eight_bit[size] = { 0,0,0,0,0,0,0,0 };
    while (1) {

            switch (buffer[IP]) {
                case 0x00 :
                    if (eight_bit_search(buffer, IP+1) == true) {

                        for (int i = 0; i < size; i++) {

                            if (eight_bit[i] == 0) {
                                eight_bit[i] = eight_bit[i] + buffer[IP];
                                secure = true;
                            }
                        }
                        if (secure == false)
                            eight_bit[0] = eight_bit[0] + buffer[IP];
                    }
                    else
                        eight_bit[0] = eight_bit[0] + buffer[IP];
                    Flags_1();
                    break;

                case 0x01 :
                    if (sixteen_bit_search(buffer, IP + 1) == true) {

                        for (int i = 0; i < (size/2); i++) {

                            if (regs[i] == 0) {
                                regs[i] = regs[i] + buffer[IP];
                                secure = true;
                            }
                        }
                        if (secure == false)
                            regs[0] = regs[0] + buffer[IP];
                    }
                    else
                        regs[0] = regs[0] + buffer[IP];
                    Flags_1();
                    break;

                case 0x02:
                    if (eight_bit_search(buffer, IP + 1) == true) {

                        for (int i = 0; i < size; i++) {

                            if (eight_bit[i] == 0) {
                                eight_bit[i] = eight_bit[i] + buffer[IP];
                                secure = true;
                            }
                        }
                        if (secure == false)
                            eight_bit[0] = eight_bit[0] + buffer[IP];
                    }
                    else
                        eight_bit[0] = eight_bit[0] + buffer[IP];
                    Flags_1();
                    break;

                case 0x03:
                    if (sixteen_bit_search(buffer, IP + 1) == true) {

                        for (int i = 0; i < (size/2); i++) {

                            if (regs[i] == 0) {
                                regs[i] = regs[i] + buffer[IP];
                                secure = true;
                            }
                        }
                        if (secure == false)
                            regs[0] = regs[0] + buffer[IP];
                    }
                    else
                        regs[0] = regs[0] + buffer[IP];
                    Flags_1();
                    break;

                 case 0x04:
                    eight_bit[0] = eight_bit[0] + buffer[IP];
                    Flags_1();
                    break;

                  case 0x05:
                    regs[0] = regs[0] + buffer[IP];
                    Flags_1();
                    break;

                  case 0x06:
                    point_reg[0] = point_reg[0] - 2;
                    break;

                  case 0x07:
                    point_reg[0] = point_reg[0] + 2;
                    break;

                  case 0x08:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] | buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] | buffer[IP];
                      }
                      else
                          eight_bit[0] = eight_bit[0] | buffer[IP];
                      Flags_2();
                      break;

                  case 0x09:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] | buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] | buffer[IP];
                      }
                      else
                          regs[0] = regs[0] | buffer[IP];
                      Flags_2();
                      break;

                  case 0x0A:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] | buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] | buffer[IP];
                      }
                      else
                          eight_bit[0] = eight_bit[0] | buffer[IP];
                      Flags_2();
                      break;

                  case 0x0B:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] | buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] | buffer[IP];
                      }
                      else
                          regs[0] = regs[0] | buffer[IP];
                      Flags_2();
                      break;

                  case 0x0C:
                      eight_bit[0] = eight_bit[0] | buffer[IP];
                      Flags_2();
                      break;

                  case 0x0D:
                      regs[0] = regs[0] + buffer[IP];
                      Flags_2();
                      break;

                  case 0x0E:
                    point_reg[1] = point_reg[1] - 2;
                    break;

                  case 0x10:

                    if (eight_bit_search(buffer, IP + 1) == true) {

                        for (int i = 0; i < size; i++) {

                            if (eight_bit[i] == 0) {
                                eight_bit[i] = eight_bit[i] + buffer[IP] + FLAGS[1];
                                  secure = true;
                            }
                        }

                        if (secure == false)
                            eight_bit[0] = eight_bit[0] + buffer[IP] + FLAGS[1];
                    }
                    else
                        eight_bit[0] = eight_bit[0] + buffer[IP] + FLAGS[1];
                    Flags_1();
                    break;

                  case 0x11:
                    if (sixteen_bit_search(buffer, IP + 1) == true) {

                        for (int i = 0; i < (size/2); i++) {

                            if (regs[i] == 0) {
                                  regs[i] = regs[i] + buffer[IP] + FLAGS[1];
                                  secure = true;
                            }
                        }
                        if (secure == false)
                              regs[0] = regs[0] + buffer[IP] + FLAGS[1];
                    }
                    else
                          regs[0] = regs[0] + buffer[IP] + FLAGS[1];
                    Flags_1();
                    break;

                  case 0x12:
                    if (eight_bit_search(buffer, IP + 1) == true) {

                        for (int i = 0; i < size; i++) {

                            if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] + buffer[IP] + FLAGS[1];
                                  secure = true;
                            }
                        }
                        if (secure == false)
                              eight_bit[0] = eight_bit[0] + buffer[IP] + FLAGS[1];
                    }
                    else
                          eight_bit[0] = eight_bit[0] + buffer[IP] + FLAGS[1];
                    Flags_1();
                    break;

                  case 0x13:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] + buffer[IP] + FLAGS[1];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] + buffer[IP] + FLAGS[1];
                      }
                      else
                          regs[0] = regs[0] + buffer[IP] + FLAGS[1];
                      Flags_1();
                      break;

                  case 0x14:
                      eight_bit[0] = eight_bit[0] + buffer[IP] + FLAGS[1];
                      Flags_1();
                      break;

                  case 0x15:
                      regs[0] = regs[0] + buffer[IP] + FLAGS[1];
                      Flags_1();
                      break;

                  case 0x16:
                     point_reg[2] = point_reg[2] - 2;
                     break;

                  case 0x17:
                      point_reg[2] = point_reg[2] + 2;
                      break;

                  case 0x18:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] - (buffer[IP] + FLAGS[1]);
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] - (buffer[IP] + FLAGS[1]);
                      }
                      else
                          eight_bit[0] = eight_bit[0] - (buffer[IP] + FLAGS[1]);
                      Flags_1();
                      break;

                  case 0x19:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] - (buffer[IP] + FLAGS[1]);
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] - (buffer[IP] + FLAGS[1]);
                      }
                      else
                          regs[0] = regs[0] - (buffer[IP] + FLAGS[1]);
                      Flags_1();
                      break;


                  case 0x1A:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] - (buffer[IP] + FLAGS[1]);
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] - (buffer[IP] + FLAGS[1]);
                      }
                      else
                          eight_bit[0] = eight_bit[0] - (buffer[IP] + FLAGS[1]);
                      Flags_1();
                      break;

                  case 0x1B:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] - (buffer[IP] + FLAGS[1]);
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] - (buffer[IP] + FLAGS[1]);
                      }
                      else
                          regs[0] = regs[0] - (buffer[IP] + FLAGS[1]);
                      Flags_1();
                      break;

                  case 0x1C:
                      eight_bit[0] = eight_bit[0] - (buffer[IP] + FLAGS[1]);
                      Flags_1();
                      break;

                  case 0x1D:
                      regs[0] = regs[0] - (buffer[IP] + FLAGS[1]);
                      Flags_1();
                      break;

                  case 0x1E:
                     point_reg[3] = point_reg[3] - 2;
                     break;

                  case 0x1F:
                      point_reg[3] = point_reg[3] + 2;
                      break;

                  case 0x20:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] & buffer[IP] ;
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] & buffer[IP];
                      }
                      else
                          eight_bit[0] = eight_bit[0] & buffer[IP];
                      Flags_2();
                      break;

                  case 0x21:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] & buffer[IP];
                                  secure = true;
                              }
                          }

                          if (secure == false)
                              regs[0] = regs[0] & buffer[IP];
                      }
                      else
                          regs[0] = regs[0] & buffer[IP];
                      Flags_2();
                      break;

                  case 0x22:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] & buffer[IP];
                                  secure = true;
                              }
                          }

                          if (secure == false)
                              eight_bit[0] = eight_bit[0] & buffer[IP];
                      }
                      else
                          eight_bit[0] = eight_bit[0] & buffer[IP];
                      Flags_2();
                      break;

                  case 0x23:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] & buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] & buffer[IP];
                      }
                      else
                          regs[0] = regs[0] & buffer[IP];
                      Flags_2();
                      break;

                  case 0x24:
                      eight_bit[0] = eight_bit[0] & buffer[IP];
                      Flags_2();
                      break;

                  case 0x25:
                      regs[0] = regs[0] & buffer[IP];
                      Flags_2();
                      break;

                  case 0x28:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] - buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] - buffer[IP];
                      }
                      else
                          eight_bit[0] = eight_bit[0] - buffer[IP];
                      Flags_1();
                      break;

                  case 0x29:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] - buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] - buffer[IP];
                      }
                      else
                          regs[0] = regs[0] - buffer[IP];
                      Flags_1();
                      break;

                  case 0x2A:
                      if (eight_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < size; i++) {

                              if (eight_bit[i] == 0) {
                                  eight_bit[i] = eight_bit[i] - buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              eight_bit[0] = eight_bit[0] - buffer[IP];
                      }
                      else
                          eight_bit[0] = eight_bit[0] - buffer[IP];
                      Flags_1();
                      break;

                  case 0x2B:
                      if (sixteen_bit_search(buffer, IP + 1) == true) {

                          for (int i = 0; i < (size/2); i++) {

                              if (regs[i] == 0) {
                                  regs[i] = regs[i] - buffer[IP];
                                  secure = true;
                              }
                          }
                          if (secure == false)
                              regs[0] = regs[0] - buffer[IP];
                      }
                      else
                          regs[0] = regs[0] - buffer[IP];
                      Flags_1();
                      break;

                  case 0x2C:
                      eight_bit[0] = eight_bit[0] - buffer[IP];
                      Flags_1();
                      break;

                  case 0x2D:
                      regs[0] = regs[0] - buffer[IP];
                      Flags_1();
                      break;

                  case 0x40:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x41:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x42:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x43:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x44:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x45:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x46:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x47:
                      regs[0] = regs[0] + 1;
                      break;

                  case 0x48:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x49:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x4A:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x4B:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x4C:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x4D:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x4E:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x4F:
                      regs[0] = regs[0] - 1;
                      break;

                  case 0x50:
                      point_reg[0] = point_reg[0] - 2;
                      break;
                  case 0x51:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x52:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x53:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x54:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x55:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x56:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x57:
                      point_reg[0] = point_reg[0] - 2;
                      break;

                  case 0x58:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x59:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x5A:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x5B:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x5C:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x5D:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x5E:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  case 0x5F:
                      point_reg[0] = point_reg[0] + 2;
                      break;

                  default :
                      Flags_1();
                      break;
            }
            IP++;
            break;
    }

    cout << "In order AX, CX, DX, BX" << endl;
    for (int k = 0; k < (size/2); k++)
        cout << regs[k] << " ";

    cout << endl << "In order: AL, CL, DL, BL, AH, CH, DH, BH" << endl;
    for (int k = 0; k < size; k++)
        cout << eight_bit[k] << " ";

    cout << endl << "In order: ES, CS, SS, DS" << endl;
    for (int k = 0; k < (size/2); k++)
        cout << point_reg[k] << " ";

    cout << endl << "In order: ZF, CF, SF" << endl;
    for (int k = 0; k < 3; k++)
        cout << FLAGS[k] << " ";

    fclose(file_open);
    return 0;
}
