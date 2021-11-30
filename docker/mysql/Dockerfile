FROM mysql:5.7.30

# Update and clean apt cache
RUN apt-get clean && apt-get update --fix-missing && \
    # Install utils \
    apt install -y nano ssh inetutils-ping net-tools htop telnet curl redis-tools ncdu rsync whois dnsutils git zip gosu wget percona-toolkit;

# MySQL Custom Configs
COPY my.cnf /etc/mysql/
COPY mysql.conf.d/ /etc/mysql/mysql.conf.d/

# Touching log files
RUN touch -a /var/log/mysql/error.log && chown mysql:adm /var/log/mysql/error.log
RUN touch -a /var/log/mysql/general.log && chown mysql:adm /var/log/mysql/general.log
RUN touch -a /var/log/mysql/slow.log && chown mysql:adm /var/log/mysql/slow.log

# Prepare database
COPY init_db.sql /etc/mysql/init_db.sql
COPY init_db.sh /etc/mysql/init_db.sh
COPY print_users.sql /etc/mysql/print_users.sql
COPY print_users.sh /etc/mysql/print_users.sh
CMD chmod +x /etc/mysql/init_db.sh && chmod +x /etc/mysql/print_users.sh