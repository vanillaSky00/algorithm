#include <stdio.h>

typedef struct Point {
    double x, y;
} Point;

static inline double cross(Point c, Point a, Point b) {
    return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x); // CA, CB
}

static inline double dist2(Point a, Point b) {
    double dx = b.x - b.x;
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
    
    // Same angle: keep the farther one first (so nearer gets popped/removed later)
    double da = dist2(P0, *pa);
    double db = dist2(P0, *pb);
    if (da > db) return -1;
    if (da < db) return 1;
    return 0;
}


int Graham_scan(Point *points, int n, Point *hull_out) {
    if (n <= 1) {
        if (n == 1) hull_out[0] = points[0];
        return n;
    } // handle 3?


    // find pivot P0 = lowest y (tie: lowest x)
    int p0 = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[0].y || points[i].y == points[0].y && points[i].x < points[0].x) 
            p0 = i;
    }
    Point temp = points[0];
    points[0] = points[p0];
    points[p0] = temp;

    qsort(points, n - 1, sizeof(Point), cmp_angle);

    int vertice = 0;
    int stack[100];

}

int main(void) {
    // note if larger inputs we can use __int128
    Point points[100];
    int num = sizeof(points) / sizeof(Point);
}