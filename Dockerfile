FROM zjuchenyuan/base

RUN apt-get update && apt-get install -y git build-essential sudo && \
 	mkdir -p /workdir/qsym

WORKDIR /workdir/qsym
COPY . /workdir/qsym

RUN ./setup.sh
RUN pip install .

COPY --from=zjuchenyuan/afl /afl /afl