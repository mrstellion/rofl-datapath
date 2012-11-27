#!/bin/sh

cat <<-EOF
/**
 * automatically generated by $0 $*
 * do not edit
 */

enum matching_algorithm_available {
EOF

for ALG in "$@"; do
	echo "	matching_algorithm_$ALG,"
done

echo "	matching_algorithm_count,"
cat <<-EOF
};

#define MATCHING_ALGORITHM_NAMES \\
{ \\
EOF

for ALG in "$@"; do
	echo "	\"$ALG\", \\"
done

echo "}"