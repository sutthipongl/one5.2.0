# ERF

- OS username/passwd : oneadmin/oneadmin
- MySQL username/passwd : oneadmin/oneadmin
- Start script : /home/oneadmin/one5.2.0/start
- ReStart script : /home/oneadmin/one5.2.0/restart
- Stop script : /home/oneadmin/one5.2.0/stop




# OpenNebula

Original guild
https://github.com/OpenNebula/one/blob/master/README.md

# ERF installation script in Ubuntu 16.04

#sudo rm -rf /var/lib/apt/lists/*
#sudo rm /var/cache/apt/archives/lock
#sudo rm /var/lib/dpkg/lock
#sudo dpkg --configure -a
# Clipboard support
#sudo apt-get install virtualbox-guest-dkms
#sudo apt-get install open-vm-tools
#sudo apt-get install open-vm-tools-desktop
#sudo apt-get -y install openssh-server
#sudo apt-get install openssh-client
#sudo apt-get install qemu-kvm libvirt-bin
sudo apt-get -y install git
git config --global user.email "suttihpong.lertmanorat@gmail.com"
git config --global user.name "sutthipongl"
git clone https://github.com/sutthipongl/one5.2.0
sudo apt-get -y install scons
sudo apt-get -y install ruby-dev
sudo apt-get -y install libxmlrpc-c++8-dev
sudo apt-get -y install libxml2-dev
sudo apt-get -y install libssl-dev
sudo apt-get -y install libsqlite3-dev
sudo apt-get -y install collectd-core
#sudo apt-get -y install mysql-server
#sudo apt-get -y install mysql-workbench
sudo apt-get -y install nodejs-legacy
sudo apt-get -y install npm
sudo npm install -g bower
sudo npm install -g grunt
sudo npm install -g grunt-cli
cd ~/one5.2.0/src/sunstone/public
npm install
bower install
#cd ~/one5.2.0/src/sunstone/public/css
#grunt sass
#grunt requirejs
sudo ~/one5.2.0/share/install_gems/install_gems
sudo cp ~/one5.2.0/share/pkgs/Ubuntu-systemd/*.service /lib/systemd/system
sudo touch /etc/logrotate.d/opennebula
rm -rf ~/.one
mkdir ~/.one
echo "oneadmin:oneadmin" > ~/.one/one_auth
cd ~/one5.2.0
scons mysql=yes sunstone=yes new_xmlrpc=yes
sudo ./install.sh -u oneadmin -g oneadmin
rm -rf /var/lib/one/.one
mkdir /var/lib/one/.one
echo "oneadmin:oneadmin" > /var/lib/one/.one/sunstone_auth
systemctl enable opennebula
systemctl enable opennebula-sunstone
systemctl enable opennebula-econe
systemctl enable opennebula-gate
systemctl enable opennebula-novnc
systemctl enable opennebula-scheduler
systemctl enable ers
service opennebula restart
service opennebula-sunstone restart
#service opennebula start
#service opennebula-sunstone start
#systemctl status opennebula



