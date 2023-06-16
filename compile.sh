# m = 64*1024; s = 200 * 1024 * 1024; Math.floor(s/m)*m;

mkdir -p bin

includes=$(find ./physx/ -name '*.h' | xargs -L1 -I{} dirname "{}" | sort | uniq | sed -e 's/^/-I/')
cpps=$(find ./physx/source -name '*.cpp' | grep -v ExtParticleExt | grep -v ExtMetaData | grep -v FdUnixSocket | grep -v serialization | grep -v device | grep -v windows)

if [ ! -f physx-timestamp ]; then
  echo 'building physx...'
  echo $includes
  echo $cpps
  # get the hardware concurrency (number of cores) and use that to build
  numCores=$(getconf _NPROCESSORS_ONLN)
  # divide by 2
  numCores=$(($numCores / 2))
  echo "$cpps" | \
    xargs -L1 -P"$numCores" -I{} \
    emcc -O3 \
      $includes \
      {} \
      -DNDEBUG -DPX_SIMD_DISABLED -DPX_EMSCRIPTEN=1 -DPX_COOKING \
      -c \
    && touch physx-timestamp
fi

echo 'building main...'
emcc -s NO_EXIT_RUNTIME=1 -s TOTAL_MEMORY=209715200 -D__linux__ -s ALLOW_MEMORY_GROWTH=0 -O3 \
  $includes \
  objectize.cc \
  physics-base.cc physics.cc AnimationSystem/AnimationSystem.cc \
  *.o \
  -DNDEBUG -DPX_SIMD_DISABLED -DPX_EMSCRIPTEN=1 -DPX_COOKING \
  -I. \
  --pre-js pre.js \
  -o bin/avatars-wasm.js
sed -Ei 's/scriptDirectory\+path/"\/"+path/g' bin/avatars-wasm.js
echo 'let accept, reject;const p = new Promise((a, r) => {  accept = a;  reject = r;});Module.postRun = () => {  accept();};Module.waitForLoad = () => p;run();export default Module;' >> bin/avatars-wasm.js
echo 'done building main'
