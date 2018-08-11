#pragma once

#include<cstdio>
#include<string>
using namespace std;
struct pixel {
	pixel() {}
	pixel(double r, double g, double b) {
		if (g > 1.1 || r > 1.1 || b > 1.1) { printf("color out\n"); system("pause"); }
		c[0] = b * 255;
		c[1] = g * 255;
		c[2] = r * 255;
	}
	pixel operator*(double k) {
		pixel t;
		if (c[0] * k > 255 || c[1] * k > 255 || c[2] * k > 255) { printf("* out\n"); system("pause"); }
		t.c[0] = c[0] * k;
		t.c[1] = c[1] * k;
		t.c[2] = c[2] * k;

		return t;
	}

	pixel operator+(pixel k) {
		pixel t;
		t.c[0] = c[0] + k.c[0];
		t.c[1] = c[1] + k.c[1];
		t.c[2] = c[2] + k.c[2];

		return t;
	}

	unsigned char c[3];//0B  1G  2R
};


struct Color {
	Color() {}
	Color(double _r, double _g, double _b) {
		r = _r;
		g = _g;
		b = _b;
	}
	pixel GPixel() {
		return pixel(r, g, b);
	}
	Color operator*(double k) {
		return Color(r*k, g*k, b*k);
	}

	void operator*=(double k) {
		g *= k;
		r *= k;
		b *= k;
	}

	Color operator+(Color k) {
		return Color(r + k.r, g + k.g, b + k.b);
	}

	void operator+=(Color k) {
		g += k.g;
		r += k.r;
		b += k.b;
	}

	double g, r, b;
};

class mypic {
public:
	mypic(int height, int length) {
		int i;
		hei = height;
		len = length;
		data = (pixel**)malloc(sizeof(pixel*)*height);
		for (i = 0; i<height; i++)
			data[i] = (pixel*)malloc(sizeof(pixel)*length);
	}
	~mypic() {}
	void ReadBmp(FILE *fp);
	int GLen();
	int GHei();
	unsigned char RD(int i, int j);
	unsigned char GR(int i, int j);
	unsigned char BL(int i, int j);

	void putpixel(int i, int j, pixel t);
	pixel getpixel(int i, int j);
private:
	int hei, len;
	pixel **data;
};

inline int mypic::GHei() {
	return hei;
}


inline int mypic::GLen() {
	return len;
}

inline unsigned char mypic::RD(int i, int j) {
	return data[i][j].c[2];
}

inline unsigned char mypic::GR(int i, int j) {
	return data[i][j].c[1];
}

inline unsigned char mypic::BL(int i, int j) {
	return data[i][j].c[0];
}

inline void mypic::putpixel(int i, int j, pixel t) {
	data[i][j] = t;
}

inline pixel mypic::getpixel(int i, int j) {
	return data[i][j];
}

void picout(mypic &p, FILE *fp, string path = "out") {
	path += ".ppm";
	fopen_s(&fp, path.c_str(), "wb");
	fprintf_s(fp, "P6\n%d %d\n255\n", p.GLen(), p.GHei());
	static unsigned char color[3];
	for (int j = p.GHei() - 1; j >= 0; j--)
		for (int i = 0; i<p.GLen(); i++){

			color[0] = p.RD(j, i) & 255;
			color[1] = p.GR(j, i) & 255;
			color[2] = p.BL(j, i) & 255;
			
			fwrite(color, 1, 3, fp);
		}

	fclose(fp);
}

// end of picture processing