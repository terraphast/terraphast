# terraces
C++ data checker for the existence of terraces in phylogenetic tree space.

**Usage**: `terraces/build/release/app <nwk file> <gene/site file>`

Program output states some imput data properties, the species whose leaf edge is used as a new tree root, and the resulting supertree in enhanced newick format. (STILL SUBJECT TO CHANGE!)

**Enhanced Newick Format**: The resulting supertree representation cann be plain Newick, but can also contain the following two notation enhancements:
- `{a,b,c}` represents any conceivable binary subtree comprising the taxa a, b, and c.
- `(A|B,C|D)` represents any conceivable binary subtree comprising either subtrees A or B on the left, and either subtrees C or D on the right branch.

Both enhancements were chosen such that the result is standard newick format if there's only one possible supertree.