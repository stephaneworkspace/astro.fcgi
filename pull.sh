#!/bin/sh
if [ -n "$(git status --porcelain)" ]; then
  echo "GIT: there are changes";
else
  if [ -d "build" ]; then
    rm -rf build
  fi
  cd build
  cmake ..
  make
  mv astro.fcgi ../bin
  git add .
  git commit -m "build"
  git push
  ssh ubuntu@astrologie-traditionnelle.net "sudo rm -rf /home/ubuntu/www/astro.fcgi && cd /home/ubuntu/www && git clone git@github.com:stephaneworkspace/astro.fcgi.git && cd /home/ubuntu/www/astro.fcgi && git pull && sudo rm -rf /var/www/astro.fcgi/bin && sudo cp -r /home/ubuntu/www/astro.fcgi/bin /var/www/astro.fcgi && sudo chown -R 775 bin && sudo chown -R www-data:www-data bin"
  echo "done"
fi
