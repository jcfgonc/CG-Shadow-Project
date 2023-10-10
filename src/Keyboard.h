/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
*
* Just a container for keyboard events.
*/
#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER
class Keyboard {
public:
	bool esc;
	bool pageup, pagedown;
	bool home, end;
	bool space;
	bool up, down, left, right;
	bool q, w, e, r, t, y, u, i, o, p, a, s, d, f, g, h, j, k, l, z, x, c, v, b, n, m;
	int shift, alt, control;

	Keyboard() {
		up = 0;
		down = 0;
		left = 0;
		right = 0;
		q = 0;
		w = 0;
		e = 0;
		r = 0;
		t = 0;
		y = 0;
		u = 0;
		i = 0;
		o = 0;
		p = 0;
		a = 0;
		s = 0;
		d = 0;
		f = 0;
		g = 0;
		h = 0;
		j = 0;
		k = 0;
		l = 0;
		z = 0;
		x = 0;
		c = 0;
		v = 0;
		b = 0;
		n = 0;
		m = 0;
		esc = 0;
		pageup = 0;
		pagedown = 0;
		home = 0;
		end = 0;
		space = 0;
		shift = 0;
		alt = 0;
		control = 0;
	}
};
#endif
