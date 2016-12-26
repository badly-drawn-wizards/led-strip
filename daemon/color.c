#include <color.h>

void from_rgb_3i(uint8_t r, uint8_t g, uint8_t b, color *c) {
  c->r = r;
  c->g = g;
  c->b = b;
}

void from_rgb_3d(double r, double g, double b, color *c) {
  c->r = 0xFF * r;
  c->g = 0xFF * g;
  c->b = 0xFF * b;
}

void from_hsv_3d(double h, double s, double v, color *c) {
  if(s == 0) {
    from_rgb_3d(v, v, v, c);
    return;
  }
  h = fmod(h, 1);
  uint8_t div = h * 6;
  double frac = fmod(h*6, 1.);
  double p = v * (1.0 - s);
  double q = v * (1.0 - s * frac);
  double t = v * (1.0 - s * (1-frac));
  switch(div) {
  case 0:
    from_rgb_3d(v,t,p,c);
    break;
  case 1:
    from_rgb_3d(q,v,p,c);
    break;
  case 2:
    from_rgb_3d(p,v,t,c);
    break;
  case 3:
    from_rgb_3d(p,q,v,c);
    break;
  case 4:
    from_rgb_3d(t,p,v,c);
    break;
  case 5:
    from_rgb_3d(v,p,q,c);
    break;
  }
}
