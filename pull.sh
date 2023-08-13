#!/bin/sh
DEBUG="-DCMAKE_BUILD_TYPE=Debug"
#DEBUG=""
if [ -n "$(git status --porcelain)" ]; then
  echo "GIT: there are changes";
else
  echo "ok\"$DEBUG\"ok"
    ssh ubuntu@astrologie-traditionnelle.net "
        sudo rm -rf /home/ubuntu/www/astro.fcgi
        cd /home/ubuntu/www

        git clone git@github.com:stephaneworkspace/astro.fcgi.git
        cd /home/ubuntu/www/astro.fcgi

        git pull

        rm -rf /home/ubuntu/www/astro.fcgi/build
        mkdir /home/ubuntu/www/astro.fcgi/build

        cd /home/ubuntu/www/astro.fcgi/build
        cp /home/ubuntu/ephem/*.se1 /home/ubuntu/www/astro.fcgi/build/

        export C=/usr/bin/clang
        export CXX=/usr/bin/clang++
        cmake $DEBUG -DSW_DEBUG=0 ..
        make astro.fcgi
        cp -r /home/ubuntu/www/astro.fcgi/build/bin/astro.fcgi /home/ubuntu/www/astro.fcgi/build/astro.fcgi
        sudo rm -rf /var/www/astro.fcgi/build
        sudo cp -r /home/ubuntu/www/astro.fcgi/build /var/www/astro.fcgi

        sudo chmod -R 775 /var/www/astro.fcgi/build
        sudo chown -R www-data:www-data /var/www/astro.fcgi/build
        sudo chmod +x /var/www/astro.fcgi/build/astro.fcgi
    "

    echo "done"
fi
