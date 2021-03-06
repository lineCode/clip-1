#!/bin/bash
set -ue

if [[ ! $# -eq 1 ]]; then
  echo "usage: $0 <output_dir>" >&2
  exit 1
fi

export output_dir="$(readlink -f $1)"

if [[ ! -d "${output_dir}" ]]; then
  echo "error: directory does not exist: ${output_dir}" >&2
  exit 1
fi

cd "$(dirname "$0")/../"

cp web/documentation.css "${output_dir}"
cp web/redirects.txt "${output_dir}/_redirects"
cp web/favicon.ico "${output_dir}"
mkdir -p "${output_dir}/figures"
cp figures/*.svg "${output_dir}/figures/"
./web/build_pages.py
./web/build_examples.py
