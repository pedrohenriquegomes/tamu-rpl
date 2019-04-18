# TAMU-TSCH - Thompson sAmpling-based MUlti-channel RPL

Source code with implementation of TAMU-RPL protocol.

This code was based on [OpenWSN][2] 1.10.

This protocol is an adaptation of RPL protocol.

In TAMU-RPL, the selection of the preferred parent in the RPL protocol is modeled as a Multi-Armed Bandit (MAB) problem and the use of the Thompson Sampling heuristic is investigated to improve the reactiveness of RPL.
It was introduced in the paper below. Please cite it if you use this source code.

[2]:https://openwsn.atlassian.net/wiki/spaces/OW/overview