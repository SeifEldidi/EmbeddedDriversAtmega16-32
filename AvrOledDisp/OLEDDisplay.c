/*
 * OLEDDisplay.c
 *
 *  Created on: May 2, 2023
 *      Author: Seif pc
 */
#include "OLEDDisplay.h"

uint8 X_Cursor;
uint8 Y_Cursor;

#define FONT_SIZE 5
const unsigned char PROGMEM OledFontTable[][FONT_SIZE]={
    0x00, 0x00, 0x00, 0x00, 0x00,   // space
    0x00, 0x00, 0x2f, 0x00, 0x00,   // !
    0x00, 0x07, 0x00, 0x07, 0x00,   // "
    0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
    0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
    0x23, 0x13, 0x08, 0x64, 0x62,   // %
    0x36, 0x49, 0x55, 0x22, 0x50,   // &
    0x00, 0x05, 0x03, 0x00, 0x00,   // '
    0x00, 0x1c, 0x22, 0x41, 0x00,   // (
    0x00, 0x41, 0x22, 0x1c, 0x00,   // )
    0x14, 0x08, 0x3E, 0x08, 0x14,   // *
    0x08, 0x08, 0x3E, 0x08, 0x08,   // +
    0x00, 0x00, 0xA0, 0x60, 0x00,   // ,
    0x08, 0x08, 0x08, 0x08, 0x08,   // -
    0x00, 0x60, 0x60, 0x00, 0x00,   // .
    0x20, 0x10, 0x08, 0x04, 0x02,   // /

    0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
    0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
    0x42, 0x61, 0x51, 0x49, 0x46,   // 2
    0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
    0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
    0x27, 0x45, 0x45, 0x45, 0x39,   // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
    0x01, 0x71, 0x09, 0x05, 0x03,   // 7
    0x36, 0x49, 0x49, 0x49, 0x36,   // 8
    0x06, 0x49, 0x49, 0x29, 0x1E,   // 9

    0x00, 0x36, 0x36, 0x00, 0x00,   // :
    0x00, 0x56, 0x36, 0x00, 0x00,   // ;
    0x08, 0x14, 0x22, 0x41, 0x00,   // <
    0x14, 0x14, 0x14, 0x14, 0x14,   // =
    0x00, 0x41, 0x22, 0x14, 0x08,   // >
    0x02, 0x01, 0x51, 0x09, 0x06,   // ?
    0x32, 0x49, 0x59, 0x51, 0x3E,   // @

    0x7C, 0x12, 0x11, 0x12, 0x7C,   // A
    0x7F, 0x49, 0x49, 0x49, 0x36,   // B
    0x3E, 0x41, 0x41, 0x41, 0x22,   // C
    0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
    0x7F, 0x49, 0x49, 0x49, 0x41,   // E
    0x7F, 0x09, 0x09, 0x09, 0x01,   // F
    0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
    0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
    0x00, 0x41, 0x7F, 0x41, 0x00,   // I
    0x20, 0x40, 0x41, 0x3F, 0x01,   // J
    0x7F, 0x08, 0x14, 0x22, 0x41,   // K
    0x7F, 0x40, 0x40, 0x40, 0x40,   // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
    0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
    0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
    0x7F, 0x09, 0x09, 0x09, 0x06,   // P
    0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
    0x7F, 0x09, 0x19, 0x29, 0x46,   // R
    0x46, 0x49, 0x49, 0x49, 0x31,   // S
    0x01, 0x01, 0x7F, 0x01, 0x01,   // T
    0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
    0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
    0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
    0x63, 0x14, 0x08, 0x14, 0x63,   // X
    0x07, 0x08, 0x70, 0x08, 0x07,   // Y
    0x61, 0x51, 0x49, 0x45, 0x43,   // Z

    0x00, 0x7F, 0x41, 0x41, 0x00,   // [
    0x55, 0xAA, 0x55, 0xAA, 0x55,   // Backslash (Checker pattern)
    0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
    0x04, 0x02, 0x01, 0x02, 0x04,   // ^
    0x40, 0x40, 0x40, 0x40, 0x40,   // _
    0x00, 0x03, 0x05, 0x00, 0x00,   // `

    0x20, 0x54, 0x54, 0x54, 0x78,   // a
    0x7F, 0x48, 0x44, 0x44, 0x38,   // b
    0x38, 0x44, 0x44, 0x44, 0x20,   // c
    0x38, 0x44, 0x44, 0x48, 0x7F,   // d
    0x38, 0x54, 0x54, 0x54, 0x18,   // e
    0x08, 0x7E, 0x09, 0x01, 0x02,   // f
    0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g
    0x7F, 0x08, 0x04, 0x04, 0x78,   // h
    0x00, 0x44, 0x7D, 0x40, 0x00,   // i
    0x40, 0x80, 0x84, 0x7D, 0x00,   // j
    0x7F, 0x10, 0x28, 0x44, 0x00,   // k
    0x00, 0x41, 0x7F, 0x40, 0x00,   // l
    0x7C, 0x04, 0x18, 0x04, 0x78,   // m
    0x7C, 0x08, 0x04, 0x04, 0x78,   // n
    0x38, 0x44, 0x44, 0x44, 0x38,   // o
    0xFC, 0x24, 0x24, 0x24, 0x18,   // p
    0x18, 0x24, 0x24, 0x18, 0xFC,   // q
    0x7C, 0x08, 0x04, 0x04, 0x08,   // r
    0x48, 0x54, 0x54, 0x54, 0x20,   // s
    0x04, 0x3F, 0x44, 0x40, 0x20,   // t
    0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
    0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
    0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
    0x44, 0x28, 0x10, 0x28, 0x44,   // x
    0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y
    0x44, 0x64, 0x54, 0x4C, 0x44,   // z

    0x00, 0x10, 0x7C, 0x82, 0x00,   // {
    0x00, 0x00, 0xFF, 0x00, 0x00,   // |
    0x00, 0x82, 0x7C, 0x10, 0x00,   // }
    0x00, 0x06, 0x09, 0x09, 0x06    // ~ (Degrees)
};


const PROGMEM uint8_t ssd1306xled_font8x16[] = {
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //   0
     0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00, // ! 1
     0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " 2
     0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00, // # 3
     0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00, // $ 4
     0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00, // % 5
     0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10, // & 6
     0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' 7
     0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00, // ( 8
     0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00, // ) 9
     0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00, // * 10
     0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00, // + 11
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, // , 12
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // - 13
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, // . 14
     0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, // / 15
     0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00, // 0 16
     0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // 1 17
     0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00, // 2 18
     0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // 3 19
     0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00, // 4 20
     0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00, // 5 21
     0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // 6 22
     0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, // 7 23
     0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00, // 8 24
     0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00, // 9 25
     0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, // : 26
     0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, // ; 27
     0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, // < 28
     0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, // = 29
     0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, // > 30
     0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x36, 0x01, 0x00, 0x00, // ? 31
     0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00, 0x07, 0x18, 0x27, 0x24, 0x23, 0x14, 0x0B, 0x00, // @ 32
     0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x23, 0x02, 0x02, 0x27, 0x38, 0x20, // A 33
     0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // B 34
     0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, // C 35
     0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // D 36
     0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00, // E 37
     0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00, // F 38
     0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00, // G 39
     0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20, // H 40
     0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // I 41
     0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, 0x00, // J 42
     0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00, 0x20, 0x3F, 0x20, 0x01, 0x26, 0x38, 0x20, 0x00, // K 43
     0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00, // L 44
     0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00, 0x20, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x20, 0x00, // M 45
     0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x20, 0x00, 0x07, 0x18, 0x3F, 0x00, // N 46
     0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // O 47
     0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x01, 0x00, 0x00, // P 48
     0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x18, 0x24, 0x24, 0x38, 0x50, 0x4F, 0x00, // Q 49
     0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20, // R 50
     0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00, // S 51
     0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // T 52
     0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, // U 53
     0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x07, 0x38, 0x0E, 0x01, 0x00, 0x00, // V 54
     0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00, 0x03, 0x3C, 0x07, 0x00, 0x07, 0x3C, 0x03, 0x00, // W 55
     0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08, 0x20, 0x30, 0x2C, 0x03, 0x03, 0x2C, 0x30, 0x20, // X 56
     0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // Y 57
     0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x20, 0x38, 0x26, 0x21, 0x20, 0x20, 0x18, 0x00, // Z 58
     0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00, // [ 59
     0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x38, 0xC0, 0x00, // \ 60
     0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00, // ] 61
     0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ^ 62
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // _ 63
     0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ` 64
     0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x19, 0x24, 0x22, 0x22, 0x22, 0x3F, 0x20, // a 65
     0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // b 66
     0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x20, 0x11, 0x00, // c 67
     0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x10, 0x3F, 0x20, // d 68
     0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x22, 0x13, 0x00, // e 69
     0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // f 70
     0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6B, 0x94, 0x94, 0x94, 0x93, 0x60, 0x00, // g 71
     0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, // h 72
     0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // i 73
     0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, // j 74
     0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x24, 0x02, 0x2D, 0x30, 0x20, 0x00, // k 75
     0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // l 76
     0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x3F, 0x20, 0x00, 0x3F, // m 77
     0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, // n 78
     0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, // o 79
     0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xA1, 0x20, 0x20, 0x11, 0x0E, 0x00, // p 80
     0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0xA0, 0xFF, 0x80, // q 81
     0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x3F, 0x21, 0x20, 0x00, 0x01, 0x00, // r 82
     0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x33, 0x24, 0x24, 0x24, 0x24, 0x19, 0x00, // s 83
     0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x00, 0x00, // t 84
     0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x10, 0x3F, 0x20, // u 85
     0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x01, 0x0E, 0x30, 0x08, 0x06, 0x01, 0x00, // v 86
     0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x0F, 0x30, 0x0C, 0x03, 0x0C, 0x30, 0x0F, 0x00, // w 87
     0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x31, 0x2E, 0x0E, 0x31, 0x20, 0x00, // x 88
     0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x8E, 0x70, 0x18, 0x06, 0x01, 0x00, // y 89
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x21, 0x30, 0x2C, 0x22, 0x21, 0x30, 0x00, // z 90
     0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x40, 0x40, // { 91
     0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, // | 92
     0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, // } 93
     0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ~ 94
 };

uint8_t Image[1024];

static Bool OLED_send_cmd(uint8 Cmd)
{
	Bool E_Stat=E_OK;
	I2C_GEN_START_COND();
	I2C_WAIT();
	I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
	I2C_WRITE_BYTE(OLEDSSD_COMMAND);
	I2C_WRITE_BYTE(Cmd);
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return E_Stat;
}

static Bool OLED_NEW_line()
{
	Bool E_Stat=E_OK;
	Y_Cursor++;
	if(Y_Cursor>7)
		Y_Cursor=0;
	E_Stat=OLED_SET_CURSOR(0,Y_Cursor);
	return	E_Stat;
}

static void oled_clear_Screen();

Bool OLED_Init()
{
	Bool E_Stat=E_OK;
	I2C_CONFIG_t I2C={
			.SCL_FREQ=I2C_FAST_MODE,
			.Prescaler_Freq=TWPS_PRESCALER_1,
	};
	E_Stat=I2C_INIT(&I2C);
	DDRC |= (1 << PC2);
	PORTC &= ~(1 << PC2);
	_delay_ms(10);
	PORTC |= (1 << PC2);
	E_Stat=OLED_send_cmd(OLED_DISPLAY_OFF_SLEEP);
	E_Stat=OLED_send_cmd(OLED_SET_DISPLAY_CLK_DIVIDE);
	E_Stat=OLED_send_cmd(0x80);
	E_Stat=OLED_send_cmd(OLED_SET_MULTIPLEX_RATIO);
	E_Stat=OLED_send_cmd(0x3F);
	E_Stat=OLED_send_cmd(OLED_SET_DISPLAY_OFFSET);
	E_Stat=OLED_send_cmd(0x00);
	E_Stat=OLED_send_cmd(OLED_SET_DIPLAY_START_LINE);
	E_Stat=OLED_send_cmd(OLED_CHARGE_PUMP_SET);
	E_Stat=OLED_send_cmd(0x10);
	E_Stat=OLED_send_cmd(OLED_SET_SEG_REMAP|0x01);
	E_Stat=OLED_send_cmd(OLED_SET_COM_OUTPUT_SCAN_DIRECTION|0x08);
	E_Stat=OLED_send_cmd(OLED_SET_COM_PIN);
	E_Stat=OLED_send_cmd(0x12);
	E_Stat=OLED_send_cmd(OLED_SET_CONTRAST_CONTROL);
	E_Stat=OLED_send_cmd(0x9F);
	E_Stat=OLED_send_cmd(OLED_SET_PRE_CHARGE_PER);
	E_Stat=OLED_send_cmd(0x22);
	E_Stat=OLED_send_cmd(OLED_SET_VCOMH_DES_LEV);
	E_Stat=OLED_send_cmd(0x40);
	E_Stat=OLED_send_cmd(OLED_DISPLAY_ON_RESUME);
	E_Stat=OLED_send_cmd(OLED_NORMAL_DISPLAY);
	oled_clear_Screen();
	E_Stat=OLED_send_cmd(OLED_DISPLAY_ON);
	return E_Stat;
}

static void oled_clear_Screen()
{
	uint16 OLED_Cnt=0;
	OLED_send_cmd(OLED_SET_COLUMN_ADDR);
	OLED_send_cmd(0);//reset
	OLED_send_cmd(127);//reset
	OLED_send_cmd(OLED_SET_PAGE_ADDR);
	OLED_send_cmd(0);
	OLED_send_cmd(7);

	I2C_GEN_START_COND();
	I2C_WAIT();
	I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
	I2C_WRITE_BYTE(0x40);
	for (OLED_Cnt = 0; OLED_Cnt < 1024; OLED_Cnt++)      // Write Zeros to clear the display 128*8
	{
		I2C_WRITE_BYTE(0);
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();

	OLED_send_cmd(OLED_SET_COLUMN_ADDR);
	OLED_send_cmd(0);
	OLED_send_cmd(127);
	OLED_send_cmd(OLED_SET_PAGE_ADDR);
	OLED_send_cmd(0);
	OLED_send_cmd(7);
}

Bool OLED_SET_CURSOR(uint8 X,uint8 Y)
{
	Bool E_stat=E_OK;
	if(X>OLED_COL_SIZE||Y>OLED_PAGE_SIZE)
		E_stat=E_NOK;
	else
	{
		X_Cursor=X;
		Y_Cursor=Y;
		OLED_send_cmd(OLED_SET_COLUMN_ADDR);
		OLED_send_cmd(X);
		OLED_send_cmd(127);
		OLED_send_cmd(OLED_SET_PAGE_ADDR);
		OLED_send_cmd(Y);
		OLED_send_cmd(7);
		E_stat=E_OK;
	}
	return E_stat;
}

Bool OLED_DISPLAY_CHAR(char C)
{
	Bool E_stat=E_OK;
	uint8 Char_Cnt=0;
	if(X_Cursor>128||C=='\n')
	{
		OLED_NEW_line();
	}
	else
	{
		uint8 Char_position=C-32;
		I2C_GEN_START_COND();
		I2C_WAIT();
		I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
		I2C_WRITE_BYTE(0x40);
		for(Char_Cnt=0;Char_Cnt<5;Char_Cnt++)
		{
			I2C_WRITE_BYTE(pgm_read_byte(&OledFontTable[Char_position][Char_Cnt]));
			X_Cursor++;
		}
		I2C_GEN_STOP_COND();
		I2C_WAIT_STOP();
	}
	return E_stat;
}

static Bool OLED_DISPLAY_String_Font_2(char *str,uint8 X,uint8 Y)
{
	Bool E_stat = E_OK;
	uint8 Char_Cnt = 0;
	uint8 Font_Cnt=0;
	uint8 C;
	uint8 Old_Y=Y;
	X_Cursor=X;
	Y_Cursor=Y;
	while(*(str+Char_Cnt)!='\0')
	{
		C=*(str+Char_Cnt)-0x20;
		if(X_Cursor>120)
		{
			X_Cursor=0;
			Y_Cursor++;
			if(Y_Cursor>7)
				Y_Cursor=0;
		}
		OLED_SET_CURSOR(X_Cursor,Y_Cursor);
		I2C_GEN_START_COND();
		I2C_WAIT();
		I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
		I2C_WRITE_BYTE(0x40);
		for (Font_Cnt = 0; Font_Cnt < 8; Font_Cnt++) {
			I2C_WRITE_BYTE(pgm_read_byte(&ssd1306xled_font8x16 [C*16  + Font_Cnt]));
		}
		I2C_GEN_STOP_COND();
		I2C_WAIT_STOP();
		OLED_SET_CURSOR(X_Cursor,Y_Cursor+1);
		I2C_GEN_START_COND();
		I2C_WAIT();
		I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
		I2C_WRITE_BYTE(0x40);
		for (Font_Cnt = 0; Font_Cnt < 8; Font_Cnt++) {
			I2C_WRITE_BYTE(
					pgm_read_byte(&ssd1306xled_font8x16 [C*16+ Font_Cnt+8]));
		}
		I2C_GEN_STOP_COND();
		I2C_WAIT_STOP();
		X_Cursor+=8;
		Y_Cursor=Old_Y;
		Char_Cnt++;
	}
	return E_stat;
}

static Bool OLED_DISPLAY_String_font_1(char *str)
{
	Bool E_stat=E_OK;
	if(NULL==str)
		E_stat=E_NOK;
	else
	{
		uint8 Str_pos=0;
		while(*(str+Str_pos)!='\0')
		{
			E_stat=OLED_DISPLAY_CHAR(*(str+Str_pos));
			Str_pos++;
		}
	}
	return E_stat;
}

Bool OLED_DISPLAY_String(char *str,uint8 X,uint8 Y,uint8 Font)
{
	Bool E_stat=E_OK;
	if(Font==1)
	{
		OLED_SET_CURSOR(X,Y);
		OLED_DISPLAY_String_font_1(str);
	}
	else if(Font==2)
	{
		OLED_DISPLAY_String_Font_2(str,X,Y);
	}
	else
		E_stat=E_NOK;
	return E_stat;
}

void OLDE_ScrollRight(uint8_t Pag_start, uint8_t Pag_End)
{
	OLED_send_cmd(OLED_HORIZONTAL_SCROLL_RIGHT);
	OLED_send_cmd(0X00);
	OLED_send_cmd(Pag_start);
	OLED_send_cmd(0X00);
	OLED_send_cmd(Pag_End);
	OLED_send_cmd(0X00);
	OLED_send_cmd(0XFF);
	OLED_send_cmd(OLED_ACTIVATE_SCROLL);
}

void OLDE_ScrollLeft(uint8_t Pag_start, uint8_t Pag_End)
{
	OLED_send_cmd(OLED_HORIZONTAL_SCROLL_LEFT);
	OLED_send_cmd(0X00);
	OLED_send_cmd(Pag_start);
	OLED_send_cmd(0X00);
	OLED_send_cmd(Pag_End);
	OLED_send_cmd(0X00);
	OLED_send_cmd(0XFF);
	OLED_send_cmd(OLED_ACTIVATE_SCROLL);
}

void OLED_DEACTIVATE_scroll()
{
	OLED_send_cmd(OLED_DEACTIVATE_SCROLL);
}

static uint8 Num_length(int32 Num)
{
	uint8 Length=0;
	while(Num>0)
	{
		Length++;
		Num/=10;
	}
	return Length;
}

static void Memset(uint8 *str,uint8 Len)
{
	uint8 Cnt=0;
	while(Cnt<Len)
	{
		*(str+Cnt)='\0';
		Cnt++;
	}
}

Bool OLED_display_Integer(int32 Num,uint8 Col_C,uint8 Row_C,uint8 Font)
{
	Bool E_stat=E_OK;
	Bool Neg_flag=0;
	uint8 length=0;
	uint8 Str_Arr[30];
	uint8 Cnt=0;
	Memset(Str_Arr,30);
	if(Num<0)
	{
		Num*=-1;
		Neg_flag=1;
	}
	if(Row_C>OLED_PAGE_SIZE||Col_C>OLED_COL_SIZE-1)
		E_stat=E_NOK;
	else
	{
		OLED_SET_CURSOR(Col_C,Row_C);
		length=Num_length(Num);
		if(Neg_flag==1)
			Str_Arr[0]='-';
		else{}
		for(Cnt=0;Cnt<length;Cnt++)
		{
			if(Neg_flag==0)
				Str_Arr[length-Cnt-1]=Num%10+zero;
			else
				Str_Arr[length-Cnt]=Num%10+zero;
			Num/=10;
		}
		OLED_DISPLAY_String(Str_Arr,Col_C,Row_C,Font);
	}
	return E_stat;
}

Bool OLED_display_Float(float32 Num,uint8 Col_C,uint8 Row_C,uint8 Font)
{
	Bool E_stat = E_OK;
	Bool Neg_flag = 0;
	uint8 i_length = 0;
	uint8 f_length = 0;
	uint8 Length=0;
	uint8 Str_Arr[30];
	uint8 Cnt = 0;
	int32 Int_part=0;
	int32 Fract_part=0;
	int8 Fract;
	Memset(Str_Arr, 30);
	if(Num<0)
	{
		Num *= -1;
		Neg_flag = 1;
	}
	if(Row_C>OLED_PAGE_SIZE||Col_C>OLED_COL_SIZE-1)
			E_stat=E_NOK;
	else
	{
		Int_part=(int32)Num;
		Fract_part=(Num-Int_part)*1000000;
		i_length=Num_length(Int_part);
		f_length=Num_length(Fract_part);
		Length=i_length+f_length+1;
		OLED_SET_CURSOR(Col_C,Row_C);
		if (Neg_flag == 1)
			Str_Arr[0] = '-';
		else {
		}
		for (Cnt = 0; Cnt < f_length; Cnt++) {
			Fract=Fract_part%10;
			if (Neg_flag == 0)
			{
				if(Fract!=0)
					Str_Arr[Length - Cnt - 1] = Fract+ zero;
				else
					Str_Arr[Length - Cnt] = '\0';
			}
			else
			{
				if(Fract!=0)
					Str_Arr[Length - Cnt] = Fract+ zero;
				else
					Str_Arr[Length - Cnt] = '\0';
			}
			Fract_part /= 10;
		}
		if (Neg_flag == 0)
			Str_Arr[Length - Cnt - 1] ='.';
		else
			Str_Arr[Length - Cnt] = '.';
		Cnt++;
		for (; Cnt < Length; Cnt++) {
			if (Neg_flag == 0)
				Str_Arr[Length - Cnt - 1] = Int_part % 10 + zero;
			else
				Str_Arr[Length - Cnt] = Int_part % 10 + zero;
			Int_part /= 10;
		}
		OLED_DISPLAY_String(Str_Arr,Col_C,Row_C,Font);
	}
	return E_stat;
}

Bool OLED_SET_PIXEL(uint8 Page,uint8 Col)
{
	Bool E_stat=E_OK;
	if(Page>63||Col>OLED_COL_SIZE-1)
				E_stat=E_NOK;
	else
	{
		uint8 Pixel=Page/8;
		uint8 Pixel_pos=Page%8;
		OLED_SET_CURSOR(Col,Page);
		Image[OLED_COL_SIZE*Pixel+Col]|=(1<<Pixel_pos);
	}
	return E_stat;
}

void OLED_DISPLAY()
{
	uint8 Cnt = 0;
	uint8 L_cnt = 0;
	uint16 Seg_Cnt = 0;
	OLED_send_cmd(OLED_SET_COLUMN_ADDR);
	OLED_send_cmd(0);
	OLED_send_cmd(127);
	OLED_send_cmd(OLED_SET_PAGE_ADDR);
	OLED_send_cmd(0);
	OLED_send_cmd(7);
	for (Cnt = 0; Cnt <= OLED_PAGE_SIZE; Cnt++) {
		OLED_SET_CURSOR(0, Cnt);
		I2C_GEN_START_COND();
		I2C_WAIT();
		I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
		I2C_WRITE_BYTE(0x40);
		for (L_cnt = 0; L_cnt < OLED_COL_SIZE; L_cnt++) {
			I2C_WRITE_BYTE(Image[Seg_Cnt++]);
		}
		I2C_GEN_STOP_COND();
		I2C_WAIT_STOP();
	}
}

Bool OLED_display_Image(unsigned char *Bitmap)
{
	Bool E_stat=E_OK;
	if(Bitmap==NULL)
		E_stat=E_NOK;
	else
	{
		uint8 Cnt = 0;
		uint8 L_cnt=0;
		uint16 Seg_Cnt=0;
		OLED_send_cmd(OLED_SET_COLUMN_ADDR);
		OLED_send_cmd(0);
		OLED_send_cmd(127);
		OLED_send_cmd(OLED_SET_PAGE_ADDR);
		OLED_send_cmd(0);
		OLED_send_cmd(7);
		for (Cnt = 0; Cnt <= OLED_PAGE_SIZE; Cnt++)
		{
			OLED_SET_CURSOR(0,Cnt);
			I2C_GEN_START_COND();
			I2C_WAIT();
			I2C_WRITE_BYTE(OLEDSSD_DISPLAY_ADD);
			I2C_WRITE_BYTE(0x40);
			for(L_cnt=0;L_cnt<OLED_COL_SIZE;L_cnt++)
			{
				I2C_WRITE_BYTE(pgm_read_byte(&Bitmap[Seg_Cnt++]));
			}
			I2C_GEN_STOP_COND();
			I2C_WAIT_STOP();
		}
	}
	return E_stat;
}

void OLED_Draw_Line(uint8 x_0,uint8 y_0,uint8 x_1,uint8 y_1)
{
	uint8 steep;
	int8_t ystep;
	uint8_t delta_x, delta_y;
	int16_t err;
	steep = abs(y_1 - y_0) > abs(x_1 - x_0);
	if (steep) {
		OLED_SWAP(x_0, y_0);
		OLED_SWAP(x_1, y_1);
	}
	if (x_0 > x_1) {
		OLED_SWAP(x_0, x_1);
		OLED_SWAP(y_0, y_1);
	}
	delta_x = x_1 - x_0;
	delta_y = abs(y_1 - y_0);

	err = delta_x / 2;//mean
	if (y_0 < y_1)
		ystep = 1;//go forward by one pixel
	else
		ystep = -1;//go backward by one pixel

	for (; x_0 <= x_1; x_0++) {
		if (steep) {
			OLED_SET_PIXEL(y_0, x_0);
		} else {
			OLED_SET_PIXEL(x_0, y_0);
		}
		err -= delta_y;
		if (err < 0) {
			y_0 += ystep;
			err += delta_x;
		}
	}
}

void OLED_DRAW_HORZ_LINE(uint8 Center_x,uint8 Center_y,uint8 Width)
{
	OLED_Draw_Line(Center_y,Center_x,Center_y,Center_x+Width-1);
}

void OLED_DRAW_Vertical_LINE(uint8 Center_x,uint8 Center_y,uint8 height)
{
	OLED_Draw_Line(Center_y,Center_x,Center_y+height-1,Center_x);
}

void OLED_DRAW_RECTANGLE(uint8 x,uint8 y,uint8 Width,uint8 Height)
{
	OLED_DRAW_HORZ_LINE(x,y,Width);
	OLED_DRAW_HORZ_LINE(x,y+Height-1,Width);
	OLED_DRAW_Vertical_LINE(x,y,Height);
	OLED_DRAW_Vertical_LINE(x+Width-1,y,Height);
}

