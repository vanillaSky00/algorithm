#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point {
    double x, y;
} Point;

static inline double cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); // cross(AB, AC) = (b - a) × (c - a)
}

static inline double dist2(Point a, Point b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return dx * dx + dy * dy;
}

static Point P0;

static int cmp_angle(const void* A, const void* B) {
    // Compare angle given the pivot P[0] 
    const Point* pa = (const Point*) A;
    const Point* pb = (const Point*) B;
    
    // qsort cmp perspective: return < 0 means A < B then qsort put A first
    double cr = cross(P0, *pa, *pb);
    if (cr > 0) return -1; // pa is more CCW than pb → comes first
    if (cr < 0) return 1;  // pb comes first
    
    // Same angle: nearest first (so nearer gets popped/removed later)
    double da = dist2(P0, *pa);
    double db = dist2(P0, *pb);
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}


int graham_scan(Point *points, int n, Point *hull) {
    if (n <= 1) {
        if (n == 1) hull[0] = points[0];
        return n;
    } // handle 3?


    // find pivot P0 = lowest y (tie: lowest x)
    int p0 = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[0].y || 
           (points[i].y == points[0].y && points[i].x < points[0].x)) {
            p0 = i;
        }
    }
    Point tmp = points[0];
    points[0] = points[p0];
    points[p0] = tmp;
    P0 = points[0];

    // sort by angle exlude p0
    qsort(points + 1, n - 1, sizeof(Point), cmp_angle);

    int top = 0;
    hull[top++] = points[0];
    hull[top++] = points[1];

    for (int i = 2; i < n; i++) {
        // like next greater element problem:
        // if makes a right turn, the middle point (hull[top-1]) cannot be part of the convex hull → pop it.
        // > 0 → left turn (CCW) → put to stack
        while (top >= 2 && cross(hull[top - 2], hull[top - 1], points[i]) <= 0) {
            --top;
        }
        hull[top++] = points[i];
    }

    return top;
}

static inline double area(Point a, Point b, Point c) {
    return fabs(cross(a, b, c)) / 2.0;
}

double max_triangle_area(Point *hull, int n) {
    if (n < 3) return 0.0;

    double best = 0.0;
    for (int a = 0; a < n; a++) {
        int b = (a + 1) % n;
        int c = (b + 1) % n;

        while (b != a) {
            // fix a, b and move c
            while(area(hull[a], hull[b], hull[c]) <= area(hull[a], hull[b], hull[(c+1)%n])) {
                c = (c + 1) % n;
            }
            double A = area(hull[a], hull[b], hull[c]);
            best = (best > A) ? best : A;

            b = (b + 1) % n;
        }
    }
    return best;
}


int main(void) {
    // note if larger inputs we can use __int128
    Point pts[] = {{0,0},{2,0},{2,2},{0,2},{1,1},{1,2}};
    int n = sizeof(pts)/sizeof(pts[0]);
    Point hull[100];
    int h = graham_scan(pts, n, hull);
    for (int i = 0; i < h; ++i) {
        printf("(%.6g, %.6g)\n", hull[i].x, hull[i].y);
    }
    double max_area = max_triangle_area(hull, h);
    printf("Max Traingle Area: %.6g\n", max_area);

    // Test for nearer first
    Point pts2[] = {{0,0},{0,1},{1,0},{0,2},{2,0}};
    h = graham_scan(pts2, n, hull);
    for (int i = 0; i < h; ++i) {
        printf("(%.6g, %.6g)\n", hull[i].x, hull[i].y);
    }
    max_area = max_triangle_area(hull, h);
    printf("Max Traingle Area: %.6g\n", max_area);

}