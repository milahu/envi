echo "env1: hello"

export B=2

for i in $(seq 0 $#); do
  echo "env1: \$$i = ${!i}"
done

if [ "$1" = "--" ]; then
  shift
  echo "env1: no more env's to source -> exec"
  exec $(readlink -f $(which python)) "$@"
else
  next="$1"
  shift
  echo "env1: sourcing env2"
  echo source "$next"
fi
