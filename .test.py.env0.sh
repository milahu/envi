echo "env0: hello"

for i in $(seq 0 $#); do
  echo "env0: \$$i = ${!i}"
done

export A=1

if [ "$1" = "--" ]; then
  shift
  exec $(readlink -f $(which python)) "$@"
else
  next="$1"
  shift
  echo "env0: sourcing env1"
  source "$next"
fi

if false; then

# getting the next env file in bash is too complex
# -> find all env files in C, and pass them to bash

b=${0%.*} # get basename = remove .sh
i=${b##*.env} # get current env ID
n=$((i+1)) # get next env ID

# does the next env file exist?
next=${b%.*}.env$n.sh
if [ -e "$next" ]; then
  echo "env0: sourcing env1"
  source "$next"
else
  exec $(readlink -f $(which python)) "$@"
fi

fi
