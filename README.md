# storm-bn-spn
storm-bn-spn is a tool for analysis of (parametric) Sum Product Networks built on top of [storm-bn] and [STORM]. It exploits the probabilistic model checking techniques of STORM by transforming (parametric) SPNs into (parametric) Markov Chains, using the (p)SPN2(p)MC transformer. The (p)SPN2(p)MC transformer takes (parametric) Sum Product Networks in [BIF format] and translates them into the [Jani] specification.

# Usage
### Run the transformer
1) Make the target
```
rm -rf build
mkdir build && cd build
cmake ..
make
```
2) Run
```build/bin/storm-bn-robin```
3) Type spn or bn depending on what you want to transform and enter
```spn```
4) Type the name of the network example you want to transform and press enter.
```random1_SPFlow```

Note: Adjust the `std::string folder` in "[/src/storm-bn-robin-cli/storm-bn-robin.cpp]" to indicate the location of the examples.

# Subdirectories Description
The tool builds upon "[storm-bn/bn-mc-transformer]" without creating additional subdirectories.  It derieves from the original classes to tailor the transformation to SPNs. Class files derived from the original classes are placed under the directory "[/src/storm-bn-robin]". New classes include:
- storm-bn-robin/src/jani/JaniFileCreator_SPN.cpp
- storm-bn-robin/src/parser/ProbabilityRow_SPN.cpp
- storm-bn-robin/src/parser/ProbabilityTable_SPN.cpp
- storm-bn-robin/src/parser/SPNetwork.cpp
- storm-bn-robin/src/transformer/SPNTransformer.cpp

We modified the "[/src/storm-bn-robin-cli/storm-bn-robin.cpp]" file in storm-bn to allow users to utilize both the pSPN2pMC and existing pBN2pMC transformations

Minor changes were made to existing classes of storm-bn under "[/src/storm-bn-robin/src]" to enable the usage of derived classes.

Benchmarks are located under "src/storm-bn-robin/TopologicalOrderingsBeyza". They were created using the SPFlow library by learning SPNs from random data.

# Dependencies
- [Storm]: the backend probabilistic model checker.
- [storm-bn]: used for its (parametric) BN to MC transformer, upon which our transformation is built.

        [Storm]: <https://www.stormchecker.org/>
        [storm-bn]: <https://github.com/BaharSlmn/storm-bn/>
        [BIF format]: <http://www.cs.cmu.edu/afs/cs/user/fgcozman/www/Research/InterchangeFormat/>
        [Jani]: <https://jani-spec.org/>
        [/src/storm-bn-robin-cli/storm-bn-robin.cpp]: <https://github.com/BaharSlmn/storm-bn/blob/master/bn-mc-transformer/src/storm-bn-robin-cli/storm-bn-robin.cpp>
        [/src/storm-bn-robin/src]: <https://github.com/BaharSlmn/storm-bn/tree/master/bn-mc-transformer/src/storm-bn-robin>
        [storm-bn/bn-mc-transformer]: <https://github.com/BaharSlmn/storm-bn/tree/master/bn-mc-transformer>
        [/src/storm-bn-robin]: <https://github.com/byzakyz/storm-bn-spn/tree/main/src/storm-bn-robin>