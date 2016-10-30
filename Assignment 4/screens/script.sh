for f in *.tga; do
  convert ./"$f" ../pngs/"${f%.tga}.png"
done