#include <libart_lgpl/libart.h>
#include <math.h>
#include <stdlib.h>

double
distance(const ArtPoint *p0, const ArtPoint *p1)
{
	return sqrt(pow(p0->x - p1->x, 2) + pow(p0->y - p1->y, 2));
}
int main(int argc, char *argv[])
{
	ArtPoint p0, p1;
	ArtPoint pi0, pi1;
	double d;
	double translate[6], affine[6];

	p0.x = atoi(argv[1]);
	p0.y = atoi(argv[2]);

	p1.x = atoi(argv[3]);
	p1.y = atoi(argv[4]);

	d = distance(&p0, &p1);
	printf("distance between (%lf, %lf) and (%lf, %lf) is %lf\n",
			p0.x, p0.y, p1.x, p1.y, d);
	printf("p0 = (%lf, %lf)\tp1 = (%lf, %lf)\n", p0.x, p0.y, p1.x, p1.y);

	pi0 = p0;
	pi1 = p1;


	p1.x = p1.x - p0.x;
	p1.y = -(p1.y - p0.y);

	art_affine_translate(translate, p0.x, -p0.y);
	p0.x = p0.y = 0;


	printf("p0 = (%lf, %lf)\tp1 = (%lf, %lf)\n", p0.x, p0.y, p1.x, p1.y);
	art_affine_rotate(affine, -180 * atan2(p1.y - p0.y, p1.x - p0.x) / M_PI);
	art_affine_point(&p0, &p0, affine);
	art_affine_point(&p1, &p1, affine);
	printf("p0 = (%lf, %lf)\tp1 = (%lf, %lf)\n", p0.x, p0.y, p1.x, p1.y);

	p1 = p0;
	p1.x += atoi(argv[5]);

	art_affine_invert(affine, affine);
	art_affine_point(&p0, &p0, affine);
	art_affine_point(&p1, &p1, affine);
	printf("p0 = (%lf, %lf)\tp1 = (%lf, %lf)\n", p0.x, p0.y, p1.x, p1.y);

	p0 = pi0;
	p1.x = p1.x + pi0.x;
	p1.y = -p1.y + pi0.y;
	printf("p0 = (%lf, %lf)\tp1 = (%lf, %lf)\n", p0.x, p0.y, p1.x, p1.y);

	d = distance(&p0, &p1);
	printf("distance between (%lf, %lf) and (%lf, %lf) is %lf\n",
			p0.x, p0.y, p1.x, p1.y, d);
	return 0;
}
