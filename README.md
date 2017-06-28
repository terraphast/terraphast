# terraces
C++ data checker for the existence of terraces in phylogenetic tree space.

**Usage**: `terraces/build/release/app <nwk file> <gene/site file>`

Program output states some imput data properties, the species whose leaf edge is used as a new tree root, and the resulting supertree in enhanced newick format. (STILL SUBJECT TO CHANGE!)

**Enhanced Newick Format**: The resulting supertree representation cann be plain Newick, but can also contain the following two notation enhancements:
- `{a,b,c}` represents any conceivable binary subtree comprising the taxa a, b, and c.
- `(A|B,C|D)` represents any conceivable binary subtree comprising either subtrees A or B on the left, and either subtrees C or D on the right branch.

Both enhancements were chosen such that the result is standard newick format if there's only one possible supertree.

## Improvements and Optimizations

### Implemented:

- We introduced an optional, **compressed tree output format**. This format makes printing to terminal faster, since not all possible trees are listet in full detail. See section *Enhanced Newick Format* above. [https://git.scc.kit.edu/bioinfo2017/terraces/issues/8]
- **Memory allocation in large blocks**, and managing them with free lists. [https://git.scc.kit.edu/bioinfo2017/terraces/issues/37]
- **Deletion of unnecessary constraints**. [https://git.scc.kit.edu/bioinfo2017/terraces/issues/23]
- ...

### Planned:

- Enumerate subtrees in **parallel**. One challenge would be separation of the workload so that multiple threads have "enough to do". Another challenge would be the merging of the individual threads' results. [https://git.scc.kit.edu/bioinfo2017/terraces/issues/6]
- Finding **good heuristics** for choosing a good subtree into which we want to descend. Ideally, we'd have a nice heuristic that tells us which subtrees and associated constraints probably give us several options to construct a supertree, in which case we can safely answer "yes" to the question "are we on a terrace?". **Ideas**: "Smallest subtree first", "Least constraints first".[https://git.scc.kit.edu/bioinfo2017/terraces/issues/3]
- ...