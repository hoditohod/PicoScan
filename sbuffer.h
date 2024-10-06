#pragma once

/* SBuffer data structures. SBuffer stores spans for each scanline that primitive rasterization routines emit.
Each span contains all information neccessaŕy to render the pixels.corresponding to the span. It's an
interesting question how to store the spans in memory. Each scanline consits of spans, forming a padrtition.
When new spans are inserted exising spans may be broken into pieces, overwritten or removed. The data structure
shall support these operations efficienfly

If spans are stored in an array then insert/remove at the beginning ort the end is costly. Initial size and
growing the array raises questions as well.

A linked list looks a good choice with constant insertion and removal time. Each span (node) in the list has a next pointer which needs ~2 bytes. Spans have variable length depending on the information stored. This needs a smart memory alllocator to serve and reuse span memory.
Finding an insertion point also requires list traversal.
Another option would be to use a tree. 2 pointers per node, same memory allocator, and the added complexity of tree rotations. Finding the insertion point would take log2 time.

Combination of list and array. Spans for a scanline are stored in 64 byte bucket. Each bucket works as an array of tigthly packed spans. If one bucket is not enough to hold all spans, then the last 2 bytes contain the memory offset to the next bucket. The bucket has a 2 byte header containing the sum of span lengths (10bits) and number of free bytes (6bits).

Finding an insertion point can happen jumpig over the buckets. Insert/remove happens as with arrays, at most 62 bytes moved. If needed a new bucket is allocated.
*/



