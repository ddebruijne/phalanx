#include "DisplayBase.h"

/*
    DisplayIV12 - Noctiluca
    - IV-12 is a seven-segment display like IV-6, but lacks the dot.
    - 4 tubes.
    - Grid pin is wired to dot. if blanking, we turn off the grid too.
    - TubeDigit and CharMap are largely redefinitions of the ones in DisplayIV6 (other than enabling all last bits).
*/
class DisplayIV12 : public DisplayBase
{
protected:
    const int Digits = 4;
    volatile byte displayData[4];

public:
    bool Initialize();
    void IRAM_ATTR OnTimer();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) { ShiftCurrentTime(hour, minute, second, displayZeroFirstDigit); }
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();
};

const byte TubeDigit[10] = {
    0b11111101, // 0
    0b01100001, // 1
    0b11011011, // 2
    0b11110011, // 3
    0b01100111, // 4
    0b10110111, // 5
    0b10111111, // 6
    0b11100001, // 7
    0b11111111, // 8
    0b11110111  // 9
};

const byte CharMap[128] = {
    0b00000001, //	0	NUL
    0b00000001, //	1	SOH
    0b00000001, //	2	STX
    0b00000001, //	3	ETX
    0b00000001, //	4	EOT
    0b00000001, //	5	ENQ
    0b00000001, //	6	ACK
    0b00000001, //	7	BEL
    0b00000001, //	8	BS
    0b00000001, //	9	HT
    0b00000001, //	10	LF
    0b00000001, //	11	VT
    0b00000001, //	12	FF
    0b00000001, //	13	CR
    0b00000001, //	14	SO
    0b00000001, //	15	SI
    0b00000001, //	16	DLE
    0b00000001, //	17	DC1
    0b00000001, //	18	DC2
    0b00000001, //	19	DC3
    0b00000001, //	20	DC4
    0b00000001, //	21	NAK
    0b00000001, //	22	SYN
    0b00000001, //	23	ETB
    0b00000001, //	24	CAN
    0b00000001, //	25	EM
    0b00000001, //	26	SUB
    0b00000001, //	27	ESC
    0b00000001, //	28	FS
    0b00000001, //	29	GS
    0b00000001, //	30	RS
    0b00000001, //	31	US
    0b00000001, //	32	
    0b01100001, //	33	!
    0b00000001, //	34	"
    0b00000001, //	35	#
    0b00000001, //	36	$
    0b00000001, //	37	%
    0b00000001, //	38	&
    0b00000001, //	39	'
    0b11110001, //	40	(
    0b10011101, //	41	)
    0b00000001, //	42	*
    0b00000001, //	43	+
    0b00000001, //	44	,
    0b00000011, //	45	-
    0b00000001, //	46	.
    0b00000001, //	47	/
    0b11111101, //	48	0
    0b01100001, //	49	1
    0b11011011, //	50	2
    0b11110011, //	51	3
    0b01100111, //	52	4
    0b10110111, //	53	5
    0b10111111, //	54	6
    0b11100001, //	55	7
    0b11111111, //	56	8
    0b11110111, //	57	9
    0b00000001, //	58	:
    0b00000001, //	59	;
    0b00000001, //	60	<
    0b00010011, //	61	=
    0b00000001, //	62	>
    0b11100001, //	63	?
    0b00000001, //	64	@
    0b11101111, //	65	A
    0b00111111, //	66	B
    0b10011101, //	67	C
    0b11110011, //	68	D
    0b10011111, //	69	E
    0b10001111, //	70	F
    0b11110111, //	71	G
    0b01101111, //	72	H
    0b11000001, //	73	I
    0b11100001, //	74	J
    0b00101111, //	75	K
    0b00011101, //	76	L
    0b11101101, //	77	M
    0b00101011, //	78	N
    0b11111101, //	79	O
    0b11001111, //	80	P
    0b11100111, //	81	Q
    0b00001011, //	82	R
    0b10110111, //	83	S
    0b00011111, //	84	T
    0b01111101, //	85	U
    0b01111101, //	86	V
    0b01111101, //	87	W
    0b00110001, //	88	X
    0b01110111, //	89	Y
    0b11011011, //	90	Z
    0b11110001, //	91	[
    0b00000001, //	92	backslash
    0b10011101, //	93	]
    0b00000001, //	94	^
    0b00010001, //	95	_
    0b00000001, //	96	`
    0b11101111, //	97	a
    0b00111111, //	98	b
    0b00011011, //	99	c
    0b11110011, //	100	d
    0b11011111, //	101	e
    0b10001111, //	102	f
    0b11110111, //	103	g
    0b00101111, //	104	h
    0b01000001, //	105	i
    0b01100001, //	106	j
    0b00101111, //	107	k
    0b00011101, //	108	l
    0b11101101, //	109	m
    0b00101011, //	110	n
    0b00111011, //	111	o
    0b11001111, //	112	p
    0b11100111, //	113	q
    0b00001011, //	114	r
    0b10110111, //	115	s
    0b00011111, //	116	t
    0b00111001, //	117	u
    0b00111001, //	118	v
    0b01111101, //	119	w
    0b00110001, //	120	x
    0b01110111, //	121	y
    0b11011011, //	122	z
    0b11110001, //	123	{
    0b11000001, //	124	|
    0b10011101, //	125	}
    0b00000001, //	126	~
    0b00000001, //	127	DEL
};