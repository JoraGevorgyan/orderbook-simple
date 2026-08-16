#!/bin/sh
set -eu

IMAGE_NAME=${IMAGE_NAME:-orderbook:builder}
TARGET=${TARGET:-builder}
CPUS=${CPUS:-1}
MEMORY=${MEMORY:-1g}

echo "Building Docker image ${IMAGE_NAME} (target=${TARGET})..."
docker build --progress=plain --target "${TARGET}" -t "${IMAGE_NAME}" .

echo "Running container to build and test (cpus=${CPUS}, memory=${MEMORY})..."
docker run --rm --cpus="${CPUS}" --memory="${MEMORY}" "${IMAGE_NAME}"
EXIT=$?

if [ ${EXIT} -eq 0 ]; then
  echo "All tests passed inside container."
else
  echo "Some tests failed inside container (exit ${EXIT})."
fi

exit ${EXIT}
