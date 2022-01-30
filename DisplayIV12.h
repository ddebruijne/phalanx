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
    void IRAM_ATTR OnTick();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) { ShiftCurrentTime(hour, minute, second, displayZeroFirstDigit); }
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();
};

static byte TubeDigit[10] = {
    B11111101, // 0
    B01100001, // 1
    B11011011, // 2
    B11110011, // 3
    B01100111, // 4
    B10110111, // 5
    B10111111, // 6
    B11100001, // 7
    B11111111, // 8
    B11110111  // 9
};

static byte CharMap[128] = {
    B00000001, //	0	NUL
    B00000001, //	1	SOH
    B00000001, //	2	STX
    B00000001, //	3	ETX
    B00000001, //	4	EOT
    B00000001, //	5	ENQ
    B00000001, //	6	ACK
    B00000001, //	7	BEL
    B00000001, //	8	BS
    B00000001, //	9	HT
    B00000001, //	10	LF
    B00000001, //	11	VT
    B00000001, //	12	FF
    B00000001, //	13	CR
    B00000001, //	14	SO
    B00000001, //	15	SI
    B00000001, //	16	DLE
    B00000001, //	17	DC1
    B00000001, //	18	DC2
    B00000001, //	19	DC3
    B00000001, //	20	DC4
    B00000001, //	21	NAK
    B00000001, //	22	SYN
    B00000001, //	23	ETB
    B00000001, //	24	CAN
    B00000001, //	25	EM
    B00000001, //	26	SUB
    B00000001, //	27	ESC
    B00000001, //	28	FS
    B00000001, //	29	GS
    B00000001, //	30	RS
    B00000001, //	31	US
    B00000001, //	32	
    B01100001, //	33	!
    B00000001, //	34	"
    B00000001, //	35	#
    B00000001, //	36	$
    B00000001, //	37	%
    B00000001, //	38	&
    B00000001, //	39	'
    B11110001, //	40	(
    B10011101, //	41	)
    B00000001, //	42	*
    B00000001, //	43	+
    B00000001, //	44	,
    B00000011, //	45	-
    B00000001, //	46	.
    B00000001, //	47	/
    B11111101, //	48	0
    B01100001, //	49	1
    B11011011, //	50	2
    B11110011, //	51	3
    B01100111, //	52	4
    B10110111, //	53	5
    B10111111, //	54	6
    B11100001, //	55	7
    B11111111, //	56	8
    B11110111, //	57	9
    B00000001, //	58	:
    B00000001, //	59	;
    B00000001, //	60	<
    B00010011, //	61	=
    B00000001, //	62	>
    B11100001, //	63	?
    B00000001, //	64	@
    B11101111, //	65	A
    B00111111, //	66	B
    B10011101, //	67	C
    B11110011, //	68	D
    B10011111, //	69	E
    B10001111, //	70	F
    B11110111, //	71	G
    B01101111, //	72	H
    B11000001, //	73	I
    B11100001, //	74	J
    B00101111, //	75	K
    B00011101, //	76	L
    B11101101, //	77	M
    B00101011, //	78	N
    B11111101, //	79	O
    B11001111, //	80	P
    B11100111, //	81	Q
    B00001011, //	82	R
    B10110111, //	83	S
    B00011111, //	84	T
    B01111101, //	85	U
    B01111101, //	86	V
    B01111101, //	87	W
    B00110001, //	88	X
    B01110111, //	89	Y
    B11011011, //	90	Z
    B11110001, //	91	[
    B00000001, //	92	backslash
    B10011101, //	93	]
    B00000001, //	94	^
    B00010001, //	95	_
    B00000001, //	96	`
    B11101111, //	97	a
    B00111111, //	98	b
    B00011011, //	99	c
    B11110011, //	100	d
    B11011111, //	101	e
    B10001111, //	102	f
    B11110111, //	103	g
    B00101111, //	104	h
    B01000001, //	105	i
    B01100001, //	106	j
    B00101111, //	107	k
    B00011101, //	108	l
    B11101101, //	109	m
    B00101011, //	110	n
    B00111011, //	111	o
    B11001111, //	112	p
    B11100111, //	113	q
    B00001011, //	114	r
    B10110111, //	115	s
    B00011111, //	116	t
    B00111001, //	117	u
    B00111001, //	118	v
    B01111101, //	119	w
    B00110001, //	120	x
    B01110111, //	121	y
    B11011011, //	122	z
    B11110001, //	123	{
    B11000001, //	124	|
    B10011101, //	125	}
    B00000001, //	126	~
    B00000001, //	127	DEL
};