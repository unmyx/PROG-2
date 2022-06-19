#include <stdio.h>

double stav_motora(double h, double v, double de, double vsoft)
{
    int x = 0;
    if (h > de)
    {
        x = 0;
    }
    else if (h <= de && v <= vsoft)
        {
        x = 1;
        }
    return x;
}

double nova_rychlost(double v, double a, double dt)
{
    return v + (a * dt);
}

double nova_vyska(double h, double v, double a, double dt)
{
    return h + (v * dt) + (a * dt * dt * 0.5);
}

int main()
{

    double m, T, H, g, vsoft, dt, df, A ,de, a , v ,h, t = 0;
    int s, total = 0, max, max1 = 0, max2 = 0;

    scanf("%lf%lf%lf%lf%lf%lf", &m, &T, &H, &g, &vsoft, &dt);

    printf("m=%.3lf\nT=%.3lf\nH=%.3lf\ng=%.3lf\nvsoft=%.3lf\ndt=%.3lf\n", m, T, H, g, vsoft, dt);

    A= T / m;
    h = H;
    df = ((A - g) * H) / A;

    de = H - df;
    H = de + df;

    printf("de=%0.3lf\n", de);


  while( h > 0 )
  {
      s = stav_motora(h, v, de, vsoft);
      if (s == 1) {
          total ++;
          a = A - g;
          max1++;
          if (max1 >= max2) {
              max2 = max1;
          }
      }
      else {
          a = -g;
          max1 = 0;
      }
      printf("s=%d h=%08.3lf t=%.3lf v=%.3lf\n", s, h, t, v);

      h = nova_vyska(h, v, a, dt);
      v = nova_rychlost(v, a, dt);

      t += dt;
      if (h <= 0) {
          max = max2;
          printf("---Landed---\n");
          printf("h    =%08.3lf\nt    =%8.3lf\nv    =%8.3lf\n", h, t, v);
          printf("total=%4d\n", total);
          printf("max  =%4d", max);
          break;
      }
  }
      return 0;
}