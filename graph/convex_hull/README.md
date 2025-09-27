## What is Convex Hull ?
> The convex hull of a point set $𝑆 ⊂ 𝑅2$ is the smallest (by set inclusion) convex polygon that contains all points of 𝑆 <br>
> Equivalently, among all convex polygons containing 𝑆, it has minimum perimeter and minimum area.

Graham's scan $O(n\log n)$  angle sort + stack. <br>
Jarvis's march $O(nh)$ leftmost + wrapping. <br>
Andrew's monotone chain $O(n\log n)$  x/y sort + two sweeps. <br>
Chan's algorithm $O(n\log h)$ guess the partition of the points<br>

## Graham's scan
- Select the lowest point to begin <br>
- Sort the points by angle to lowest point in place <br>
- Only place counterclockwise point into stack 3-4 are all <br>
- If encounter clockwise then pop off the stack until making counterclockwise <br>
```
Graham_Scan(Points P):
    Input: A list P of 𝑛 2D points (𝑛 ≥ 3)
    Output: List of convex hull vertices in counter-clockwise (CCW) order.

    1. Find pivot point
        P0 = the point with the lowest y-coordinate 
        (if tie, take the leftmost among them)

    2. Sort by polar angle
        Sort all other points by polar angle with respect to P0. 
        If two points have the same angle, keep only the farthest one.

    3. Build the hull with a stack
        stack = empty
        push P0 onto stack
        push first two sorted points onto stack

        for each remaining point p in sorted list:
            while stack has at least 2 points AND
                ccw(next_to_top(stack), top(stack), p) ≤ 0:
                pop top of stack       # remove right turns or collinear
            push p onto stack

        return stack   // hull vertices in CCW order
```
### Note:
- `ccw(a,b,c)`  means cross product of vectors AB × AC.
- If result > 0 → left turn (keep).
- If result ≤ 0 → not a left turn → pop.

The same basic idea works also if the input is sorted on x-coordinate instead of angle, and the hull is computed in two steps producing the upper and the lower parts of the hull respectively. This modification was devised by A. M. Andrew. It has the same basic properties as Graham's scan.

Graham's original description involved sorting around an interior point of the convex hull, rather than one of its vertices. For the same choice of a pivot point for the sorting algorithm, connecting all of the other points in their sorted order around this point rather than performing the remaining steps of the Graham scan produces a star-shaped polygon, a polygonalization of the input.

The stack technique used in Graham's scan is very similar to that for the all nearest smaller values problem, and parallel algorithms for all nearest smaller values may also be used (like Graham's scan) to compute convex hulls of sorted sequences of points efficiently.

Reference: https://en.wikipedia.org/wiki/Graham_scan

## Jarvis's march (Gift wrapping algorithm)
- Just like graham's scan but without sorting <br>
- Select the lowest point to begin On <br>
- At each step, look at all other points and pick the one that makes the most left turn. <br>
- Repeat until you return to the starting point → hull completed.
```
Jarvis_March(Points P):
    Input: A list P of 𝑛 2D points (𝑛 ≥ 3)
    Output: List of convex hull vertices in counter-clockwise (CCW) order, with no duplicate start/end point.

    1. Pick a starting point:
        pointOnHull = the leftmost point in S
                    (if tie, pick the lowest y)
        P = empty list

    2. Wrap around the hull:
        repeat:
            add pointOnHull to P
            
            endpoint = any point in S (initial candidate)
            for each point q in S:
                if endpoint == pointOnHull
                OR (q is more counter-clockwise than endpoint
                    with respect to line (pointOnHull → endpoint)):
                    endpoint = q
            // after loop, endpoint is the most CCW point from pointOnHull

            pointOnHull = endpoint
        until pointOnHull == P[0]   // back to start

        return P   // hull vertices in CCW order
```


### Note:
$h$ is the number of vertices, so worst case is $O(n2)$ if h = n when all points given are covex hull vertices <br>
$O(nh)$ 

The inner loop checks every point in the set S, and the outer loop repeats for each point on the hull. Hence the total run time is 
$O(nh)$. The run time depends on the size of the output, so Jarvis's march is an output-sensitive algorithm.
However, because the running time depends linearly on the number of hull vertices, it is only faster than 
$O(n\log n)$ algorithms such as Graham scan when the number h of hull vertices is smaller than log n. Chan's algorithm, another convex hull algorithm, combines the logarithmic dependence of Graham scan with the output sensitivity of the gift wrapping algorithm, achieving an asymptotic running time $O(n\log h)$ that improves on both Graham scan and gift wrapping.

Reference: https://en.wikipedia.org/wiki/Gift_wrapping_algorithm

## Andrew's monotone chain
- Sort by (x, then y), build lower and upper chains by popping right turns.
```
Andrew_Monotone_Chain(Points P):
    Input: A list P of 𝑛 2D points (𝑛 ≥ 3)
    Output: List of convex hull vertices in counter-clockwise (CCW) order, with no duplicate start/end point.

    1. Sort:
        Sort P by x-coordinate. If x is equal, sort by y-coordinate.
    2. Build the lower hull (left -> right):
        L = empty list
        for each point p in P (from leftmost to rightmost):
            while L has at least 2 points AND
                the last two points in L with p make a right turn or collinear (cross ≤ 0):
                remove the last point from L
            append p to L
    3. Build the lower hull (right -> left)
        U = empty list
        for each point p in P (from rightmost to leftmost):
            while U has at least 2 points AND
                the last two points in U with p make a right turn or collinear (cross ≤ 0):
                remove the last point from U
            append p to U
    4. Merge
        Remove the last point from L
        Remove the last point from U
        Concatenate L + U → H
        return H
```


### Note:

## Chan's algorithm
Combine Graham/Andrew on small groups + Jarvis wrapping with a guessed bound ℎ If guess too small, double and retry.
```
Chan_Convex_Hull(Points P):
  Input: A set of N points P in the plane.
  Output: The vertices of the convex hull of P in counter-clockwise order.

  1. Initialize m = 2.
  2. Repeat:
     a. Divide P into approximately N/m groups, each containing at most m points.
     b. For each group, compute its convex hull using Graham's scan. Store these m-sized convex hulls.
     c. Initialize a point p_0 as the leftmost point in P.
     d. Initialize an empty list H to store the convex hull vertices. Add p_0 to H.
     e. For k = 1 to m:
        i. Let p_k-1 be the last point added to H.
        ii. For each group's convex hull, find the point that forms the largest angle (most counter-clockwise turn) with p_k-1 and the current tangent line. This can be done by finding the tangent from p_k-1 to each small convex hull.
        iii. Let p_k be the point among all candidates (from each group's tangent point) that forms the largest angle (most counter-clockwise turn) with p_k-1.
        iv. If p_k == p_0, then the convex hull is complete. Break the loop.
        v. Add p_k to H.
     f. If the loop in step 2.e completed without finding p_0 (i.e., the hull has more than m vertices), then double m (m = m^2 or m = m * m).
     g. If m becomes greater than N, set m = N.
  3. Return H.
```

### Note:
output-sensitive
not a good use in real world case


## Core math primitives
how to check if a point is clockwise or couterclockwise? we can calculate the degree but in above situation we do not care about the actual degree 
```
theta = Math.toDegrees(Math.atan2(b.y - a.y, b.x - a.x))
```
so we just use cross product of two vectors, we define the turn is from V to W

- counterclockwise turn: area is positive if V on the right of W
- clockwise ture: area is negative if V is on the left of W 
use matrix to think of the cross product

### Note: 
use 64-bit long long for the cross product to avoid overflow

## Youtube resources
https://www.youtube.com/watch?v=B2AJoQSZf4M
