#include "DisplayBase.h"

// Time position for when we don't display seconds.
// Rotating between these options gives the components some downtime and should extend lifetime.
enum TimePosition {
    Middle,
    Left,
    Right,
    Split,
    MAX
};

/*
    DisplayIV6 - Phalanx
    - IV-6 is a seven-segment display with dot, so fits perfectly in a byte. Last byte is the dot.
    - 6 tubes.
*/
class DisplayIV6 : public DisplayBase
{
protected:
    const int Digits = 6;

    TimePosition currentTimePosition = TimePosition::Middle;    // for when there's less numbers on screen than there are tubes.
    int lastHour = -1;
    int shiftOutIndex = 0;
    volatile byte displayData[6];

public:
    bool Initialize();
    void IRAM_ATTR OnTick();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();

private:
    void InternalShiftTimeComponent(int number, bool displayZeroFirstDigit, bool dotOnSecondDigit);
    void InternalShiftOut(byte data);   // emulates shiftOut() with shiftOutIndex on displayData.
};

const byte TubeDigit[10] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110  // 9
};

const byte CharMap[128] = {
    0b00000000, //	0	NUL
    0b00000000, //	1	SOH
    0b00000000, //	2	STX
    0b00000000, //	3	ETX
    0b00000000, //	4	EOT
    0b00000000, //	5	ENQ
    0b00000000, //	6	ACK
    0b00000000, //	7	BEL
    0b00000000, //	8	BS
    0b00000000, //	9	HT
    0b00000000, //	10	LF
    0b00000000, //	11	VT
    0b00000000, //	12	FF
    0b00000000, //	13	CR
    0b00000000, //	14	SO
    0b00000000, //	15	SI
    0b00000000, //	16	DLE
    0b00000000, //	17	DC1
    0b00000000, //	18	DC2
    0b00000000, //	19	DC3
    0b00000000, //	20	DC4
    0b00000000, //	21	NAK
    0b00000000, //	22	SYN
    0b00000000, //	23	ETB
    0b00000000, //	24	CAN
    0b00000000, //	25	EM
    0b00000000, //	26	SUB
    0b00000000, //	27	ESC
    0b00000000, //	28	FS
    0b00000000, //	29	GS
    0b00000000, //	30	RS
    0b00000000, //	31	US
    0b00000000, //	32	
    0b01100001, //	33	!
    0b00000000, //	34	"
    0b00000000, //	35	#
    0b00000000, //	36	$
    0b00000000, //	37	%
    0b00000000, //	38	&
    0b00000000, //	39	'
    0b11110000, //	40	(
    0b10011100, //	41	)
    0b00000000, //	42	*
    0b00000000, //	43	+
    0b00000001, //	44	,
    0b00000010, //	45	-
    0b00000001, //	46	.
    0b00000000, //	47	/
    0b11111100, //	48	0
    0b01100000, //	49	1
    0b11011010, //	50	2
    0b11110010, //	51	3
    0b01100110, //	52	4
    0b10110110, //	53	5
    0b10111110, //	54	6
    0b11100000, //	55	7
    0b11111110, //	56	8
    0b11110110, //	57	9
    0b00000000, //	58	:
    0b00000000, //	59	;
    0b00000000, //	60	<
    0b00010010, //	61	=
    0b00000000, //	62	>
    0b11100001, //	63	?
    0b00000000, //	64	@
    0b11101110, //	65	A
    0b00111110, //	66	B
    0b10011100, //	67	C
    0b11110010, //	68	D
    0b10011110, //	69	E
    0b10001110, //	70	F
    0b11110110, //	71	G
    0b01101110, //	72	H
    0b11000000, //	73	I
    0b11100000, //	74	J
    0b00101110, //	75	K
    0b00011100, //	76	L
    0b11101100, //	77	M
    0b00101010, //	78	N
    0b11111100, //	79	O
    0b11001110, //	80	P
    0b11100110, //	81	Q
    0b00001010, //	82	R
    0b10110110, //	83	S
    0b00011110, //	84	T
    0b01111100, //	85	U
    0b01111100, //	86	V
    0b01111100, //	87	W
    0b00110000, //	88	X
    0b01110110, //	89	Y
    0b11011010, //	90	Z
    0b11110000, //	91	[
    0b00000000, //	92	backslash
    0b10011100, //	93	]
    0b00000000, //	94	^
    0b00010000, //	95	_
    0b00000000, //	96	`
    0b11101110, //	97	a
    0b00111110, //	98	b
    0b00011010, //	99	c
    0b11110010, //	100	d
    0b11011110, //	101	e
    0b10001110, //	102	f
    0b11110110, //	103	g
    0b00101110, //	104	h
    0b01000000, //	105	i
    0b01100000, //	106	j
    0b00101110, //	107	k
    0b00011100, //	108	l
    0b11101100, //	109	m
    0b00101010, //	110	n
    0b00111010, //	111	o
    0b11001110, //	112	p
    0b11100110, //	113	q
    0b00001010, //	114	r
    0b10110110, //	115	s
    0b00011110, //	116	t
    0b00111000, //	117	u
    0b00111000, //	118	v
    0b01111100, //	119	w
    0b00110000, //	120	x
    0b01110110, //	121	y
    0b11011010, //	122	z
    0b11110000, //	123	{
    0b11000000, //	124	|
    0b10011100, //	125	}
    0b00000000, //	126	~
    0b00000000, //	127	DEL
};