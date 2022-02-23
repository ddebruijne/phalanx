#include "DisplayBase.h"

union iv4Data { uint32_t int32; uint8_t int8[4]; };

/*
    DisplayIV4 - Ameise
    - IV-12 is a 18-segment display (including dots), including 2 dead pins on the HV5182, we need 20 bits per tube.
    - 8 tubes.

    - -     0   LDP         10  B
   |\|/|    1   G           11  O
    - -     2   F           12  M
   |/|\|    3   P           13  L
   .- -.    4   R           14  K
            5   S           15  N
            6   E           16  A
            7   D           17  H  
            8   RDP         18  Not Connected
            9   C           19  Not Connected
*/
class DisplayIV4 : public DisplayBase
{
public:
    const bool RequiresTimer = false;

protected:
    const int Digits = 8;
    const int DisplayBytes = 20; // 20 bits * 8 tubes = 160 bits = 20 bytes.
    const int ScrollIntervalMs = 200;
    const int ScrollStartEndDelayMs = 2000;

    volatile byte displayData[20];

    // Display text with scrolling support - actively scrolling when maxOffset > 0
    String displayText;
    bool useDisplayText = false;
    bool isScrolling = false;
    int textOffset = 0;
    int maxOffset = 0;
    int msSinceScrollTick = 0;

public:
    bool Initialize();
    void OnTimer() {};
    void OnTick(uint8_t displayModeDelay);
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();

private:
    void InternalShiftDigit(const uint32_t &tubeDigit); // Shifts one value from TubeDigit or CharMap
    void InternalShiftTimeComponent(int number, bool displayZeroFirstDigit); // Shifts a digit, optionally prefixing a 0 if < 10
    void InternalCommit(); // Submits the data from DisplayData into the shift register
    void ResetDisplayText(); // 
};

const uint32_t TubeDigit[10] = {
    // NOTE:    uint32_t is 32bits, and our shift register is 20bits. 12 bits get added to the front!
    //          2 bytes are also omitted because those are not connected.
    //HANKLMOBC.DESRPFG.
    0b110000011011000110, // 0
    0b000000001010000000, // 1
    0b011000111001000110, // 2
    0b011000111011000100, // 3
    0b101000101010000000, // 4
    0b111000110011000100, // 5
    0b111000110011000110, // 6
    0b010000011010000000, // 7
    0b111000111011000110, // 8
    0b111000111011000100, // 9
};

const uint32_t CharMap[128] = {
    // NOTE:    uint32_t is 32bits, and our shift register is 20bits. 12 bits get added to the front!
    //          2 bytes are also omitted because those are not connected.
    //HANKLMOBC.DESRPFG.
    0b000000000000000000, //	0	NUL
    0b000000000000000000, //	1	SOH
    0b000000000000000000, //	2	STX
    0b000000000000000000, //	3	ETX
    0b000000000000000000, //	4	EOT
    0b000000000000000000, //	5	ENQ
    0b000000000000000000, //	6	ACK
    0b000000000000000000, //	7	BEL
    0b000000000000000000, //	8	BS
    0b000000000000000000, //	9	HT
    0b000000000000000000, //	10	LF
    0b000000000000000000, //	11	VT
    0b000000000000000000, //	12	FF
    0b000000000000000000, //	13	CR
    0b000000000000000000, //	14	SO
    0b000000000000000000, //	15	SI
    0b000000000000000000, //	16	DLE
    0b000000000000000000, //	17	DC1
    0b000000000000000000, //	18	DC2
    0b000000000000000000, //	19	DC3
    0b000000000000000000, //	20	DC4
    0b000000000000000000, //	21	NAK
    0b000000000000000000, //	22	SYN
    0b000000000000000000, //	23	ETB
    0b000000000000000000, //	24	CAN
    0b000000000000000000, //	25	EM
    0b000000000000000000, //	26	SUB
    0b000000000000000000, //	27	ESC
    0b000000000000000000, //	28	FS
    0b000000000000000000, //	29	GS
    0b000000000000000000, //	30	RS
    0b000000000000000000, //	31	US
    0b000000000000000000, //	32	
    0b000000001110000000, //	33	!
    0b100010000000000000, //	34	"
    0b000000000000000000, //	35	#
    0b111010110011010100, //	36	$
    0b111011100010011000, //	37	%
    0b111010000000111100, //	38	&
    0b100000000000000000, //	39	'
    0b000010010001010000, //	40	(
    0b010010000000010100, //	41	)
    0b000000000000000000, //	42	*
    0b001010100000010000, //	43	+
    0b000000000000000001, //	44	,
    0b001000100000000000, //	45	-
    0b000000000000000001, //	46	.
    0b000001000000001000, //	47	/
    0b110000011011000110, //	48	0
    0b000000001010000000, //	49	1
    0b011000111001000110, //	50	2
    0b011000111011000100, //	51	3
    0b101000101010000000, //	52	4
    0b111000110011000100, //	53	5
    0b111000110011000110, //	54	6
    0b010000011010000000, //	55	7
    0b111000111011000110, //	56	8
    0b111000111011000100, //	57	9
    0b000000000000000000, //	58	:
    0b000000000000000000, //	59	;
    0b000001000000100000, //	60	<
    0b001000100001000100, //	61	=
    0b000100000000001000, //	62	>
    0b000010011110000000, //	63	?
    0b000000000000000000, //	64	@
    0b111000111010000010, //	65	A
    0b111001010001100110, //	66	B
    0b110000010001000110, //	67	C
    0b110000011011000110, //	68	D
    0b111000110001000110, //	69	E
    0b111000110000000010, //	70	F
    0b110000110011000110, //	71	G
    0b101000101010000010, //	72	H
    0b010010010001010100, //	73	I
    0b000000001011000100, //	74	J
    0b101001000000100010, //	75	K
    0b100000000001000110, //	76	L
    0b100101001010000010, //	77	M
    0b100100001010100010, //	78	N
    0b110000011011000110, //	79	O
    0b111000111000000010, //	80	P
    0b110000011011100110, //	81	Q
    0b111000111000100010, //	82	R
    0b111000110011000100, //	83	S
    0b010010010000010000, //	84	T
    0b100000001011000110, //	85	U
    0b000100001010100000, //	86	V
    0b100000001010101010, //	87	W
    0b000101000000101000, //	88	X
    0b000101000000010000, //	89	Y
    0b010001010001001100, //	90	Z
    0b000010010001010000, //	91	[
    0b000100000000100000, //	92	backslash
    0b010010000000010100, //	93	]
    0b000000000000000000, //	94	^
    0b000000000001000100, //	95	_
    0b000100000000000000, //	96	`
    0b111000111010000010, //	97	a
    0b111001010001100110, //	98	b
    0b110000010001000110, //	99	c
    0b110000011011000110, //	100	d
    0b111000110001000110, //	101	e
    0b111000110000000010, //	102	f
    0b110000110011000110, //	103	g
    0b101000101010000010, //	104	h
    0b010010010001010100, //	105	i
    0b000000001011000100, //	106	j
    0b101001000000100010, //	107	k
    0b100000000001000110, //	108	l
    0b100101001010000010, //	109	m
    0b100100001010100010, //	110	n
    0b110000011011000110, //	111	o
    0b111000111000000010, //	112	p
    0b110000011011100110, //	113	q
    0b111000111000100010, //	114	r
    0b111000110011000100, //	115	s
    0b010010010000010000, //	116	t
    0b100000001011000110, //	117	u
    0b000100001010100000, //	118	v
    0b100000001010101010, //	119	w
    0b000101000000101000, //	120	x
    0b000101000000010000, //	121	y
    0b010001010001001100, //	122	z
    0b000010010001010000, //	123	{
    0b000010000000010000, //	124	|
    0b010010000000010100, //	125	}
    0b000000000000000000, //	126	~
    0b000000000000000000, //	127	DEL
};