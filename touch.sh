set -euo pipefail

echo "" >> README.md
git add .
git commit -m "lol"

for forge in $(git remote); do
  echo git push "$forge"
done
