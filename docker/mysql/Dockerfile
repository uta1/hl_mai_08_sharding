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
COPY commands.sql /etc/mysql/init.sql
COPY run_sql.sh /etc/mysql/run_sql.sh
CMD chmod +x /etc/mysql/run_sql.sh