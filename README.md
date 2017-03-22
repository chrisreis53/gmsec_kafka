# gmsec_kafka

This readme will be updated as features are added.
Currently the library has all the required 'hooks' into the GMSEC API, but functionality is coming.

Download GMSEC_API from SourceForge: [https://sourceforge.net/projects/gmsec/](https://sourceforge.net/projects/gmsec/)

Steps to get up and running:

Install the GMSEC API
Configure GMSEC According to Goddard provided documentation.

git clone gmsec_kafka into the wrapper directory (or better yet, create a symbolic link)
```
git clone https://github.com/chrisreis53/gmsec_kafka.git /path/to/GMSEC_API-4.2/wrapper
```

rename from "gmsec_kafka" to "kafka"
```
mv gmsec_kafka kafka
```

set environment variables
```
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
export MW_WRAPPERS="kafka"
```

Make the library either independently by running make in the kafka folder:
```
make -C ~/GMSEC_API-4.2/wrapper/kafka
```

Or make the whole GMSEC-API library:
```
make -C ~/GMSEC_API-4.2
```

Run GMSEC pub/sub
```
~/GMSEC_API-4.2/bin/gmpub connectiontype=gmsec_kafka
~/GMSEC_API-4.2/bin/gmsub connectiontype=gmsec_kafka
```
