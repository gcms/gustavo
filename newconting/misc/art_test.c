#include <stdio.h>
#include <libart_lgpl/libart.h>


int main(int argc, char *argv[]) {
	ArtPoint p0, p1, p;
	double rotate[6];
	double affine[6];

	p0.x = -5;
	p0.y = -20;
	p1.x = 5;
	p1.y = 20;

	art_affine_translate(affine, 50, 50);
	art_affine_rotate(rotate, 90.0);
	art_affine_multiply(affine, rotate, affine);

	p.x = 1;
	p.y = 0;

	art_affine_point(&p, &p, affine);
	printf("(%lf, %lf)\n", p.x, p.y);

	art_affine_invert(affine, affine);

	p.x = -50;
	p.y = 51;

	art_affine_point(&p, &p, affine);
	printf("(%lf, %lf)\n", p.x, p.y);

	return 0;
}
