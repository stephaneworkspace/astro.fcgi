#!/bin/sh
if [ -n "$(git status --porcelain)" ]; then
  echo "GIT: there are changes";
else
    ssh ubuntu@astrologie-traditionnelle.net "
        sudo rm -rf /home/ubuntu/www/astro.fcgi
        cd /home/ubuntu/www

        git clone git@github.com:stephaneworkspace/astro.fcgi.git
        cd /home/ubuntu/www/astro.fcgi

        git pull

        rm -rf /home/ubuntu/www/astro.fcgi/build
        mkdir /home/ubuntu/www/astro.fcgi/build

        cd /home/ubuntu/www/astro.fcgi/build

        cmake ..
        make

        sudo rm -rf /var/www/astro.fcgi/build
        sudo cp -r /home/ubuntu/www/astro.fcgi/build /var/www/astro.fcgi

        sudo chmod -R 775 /var/www/astro.fcgi/build
        sudo chown -R www-data:www-data /var/www/astro.fcgi/build
        sudo chmod +x /var/www/astro.fcgi/build/astro.fcgi
    "

    echo "done"
fi
