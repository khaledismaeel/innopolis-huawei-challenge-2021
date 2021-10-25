# Intransitive Dice

## Algorithm Description

#### Data Representation

First, note that the ternary dimensions (hereinafter referred to as *IP dimensions*) can be conveniently interpreted as a range dimension (hereinafter referred to as *port dimension*) where the length of the range is $2 ^ t$ where $t$ is the length of the trailing wild card. This allowed us to treat IP and port rules uniformly.

With that in mind, we tried to represent our data in the most cache- and compiler-friendly manner possible. Rules are represented as (up to) 5 pairs of integers corresponding to the ranges of the corresponding IP and port rules. Similarly, keys are represented as (up to) 5 integers.

```c++
namespace schema {
    const unsigned int MAXD = 5;
}

namespace data {
    const unsigned int MAXN = 150000;
    const unsigned int MAXM = 15000;
	
    array<array<pair<unsigned int, unsigned int>, schema::MAXD>, MAXN> rules;
    array<array<unsigned int, schema::MAXD>, MAXM> keys;
}
```

##### Shortcomings

It is worth noting that this representation does not exploit the fact that port ranges are much narrower than IP ranges. This is because we desired the type to be fully known at compile time and not leave any data interpretation to runtime. As a result, we have to take the worst case for range values which is 32-bits.

Ideally, if data schema was known a priori, data would be represented with appropriate `int`'s and `short`'s for IP and port rules, which would allow better caching and processing.

Also note that usage of static arrays and not vectors is rather an inconvenience. However, it proved to be marginally more efficient during sorting procedures (possible due to missing bound-checking), so we decided to stick to them.

#### Complete Search

The most straightforward possible. It was actually rather surprising that it passed preliminary tests in 2 seconds.

#### Candidate Reduction Using Scan Lines

Our optimized solutions reduces the number of possible matching rules for every key as follows. Consider an arbitrary dimension $d \in [0,5)$ in input data. The possible candidates for key $k_i$ are only those rules $r_j$ for which $r_j[d]_l \le k[d]$ and $r_j[d]_r \ge k_i[d]$. The set of such possible $r_j$ can be efficiently determined by sorting the datasets along the $d$ dimension and iterating through them in increasing order, maintaining currently opened intervals in a binary search tree.

Once the candidates for a certain key are known we can proceed with complete search as before.

##### Choice of $d$

The optimal $d$ is the $d$ which minimizes the total sum of number of possible $r_j$ for every $k_i$. It appears that IP rules are more "strict" in that regard than port rules. In the sample dataset, for an optimal $d$ an arbitrary $k_i$ has an average $\approx 1700$ candidate rules.

##### Data Structure Optimization

We observed a performance bottleneck in our implementation when using binary search trees to track opened intervals. Indeed, binary search trees are a generic data structure capable of handling arbitrary comparable data types. We decided to instead handcraft our own data structure which supports efficient insertion, deletion, and iteration of integers.

Interval ID's are in $[0,150,000)$; we can divide this interval into $\frac{150,000}{512} + 1$ chunks of $512$ integers each. We keep track of which integers within every chunk are in the data structure by a standard vector. Insertion and deletion of $x \in [0,150,000)$ are straightforward insertion and deletion to the corresponding $\frac{x}{512}$'th vector. This leads to amortized constant insertion and deletion costs and, most importantly, it's **cache-friendly**.

```c++
static array<event, 2u * data::MAXN + data::MAXM> evt;
unsigned int n_evt = 0;

for (unsigned int i = 0; i < data::n; i++)
    evt[n_evt++] = event{data::rules[i][schema::best_dim].first, i, event::type::OPEN};
for (unsigned int i = 0; i < data::m; i++)
    evt[n_evt++] = event{data::keys[i][schema::best_dim], i, event::type::KEY};
for (unsigned int i = 0; i < data::n; i++)
    evt[n_evt++] = event{data::rules[i][schema::best_dim].second, i, event::type::CLOSE};

stable_sort(evt.begin(), evt.begin() + n_evt);
static utils::bag open;

for (unsigned int i = 0; i < n_evt; i++) {
    const event &e = evt[i];
    if (e.type == event::type::OPEN)
        open.insert(e.id);
    if (e.type == event::type::KEY) {
        for (const auto &chunk: open.chunks) {
            for (auto it: chunk) {
                if (utils::match(data::rules[it], data::keys[e.id])) {
                    data::ans[e.id] = it;
                    goto escape;
                }
            }
        }
        escape:;
    }
    if (e.type == event::type::CLOSE)
        open.erase(e.id);
}
```



#### Results

![Flame chart of our optimal solution](/home/khaledismaeel/flamegraph.png)

The chunks shown above correspond to run time cost. Note that input parsing is the major performance bottleneck currently, followed by rule-key checking, sorting (although we sort only once), and then minor details including our optimized data structure cost.

#### Improvement Areas

##### Better Data Representation

Indeed, we are over-using memory and computation and under-using cache by not utilizing `short`'s.

##### Compression

Since there are at most $15000$ unique $k_i[d]$ values of along each $d$, we can compress all values (including those of rules) to the range $[0,15000)$. This will allow us to utilize counting sort and allow us to afford multiple sorts along different axes, so we can intersect candidate sets along different dimensions to further reduce runtime.

##### Faster I/O

Yeah...

#### Alternative Ideas

##### Geometric Data Structures

The problem of determining which keys match a certain rule is, in our data representation, straightforward orthogonal range search. We can utilize k-d trees, range trees, multidimensional segment trees, or whatever else. It will especially be efficient if we utilize value compression. Still, we decided not to go in that direction because the overhead of such tree data structures could ruin the execution time.
