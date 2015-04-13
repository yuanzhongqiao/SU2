/*!
 * \file mpi_structure.hpp
 * \brief In-Line subroutines of the <i>mpi_structure.hpp</i> file.
 * \author T. Albring
 * \version 3.2.9 "eagle"
 *
 * SU2 Lead Developers: Dr. Francisco Palacios (francisco.palacios@boeing.com).
 *                      Dr. Thomas D. Economon (economon@stanford.edu).
 *
 * SU2 Developers: Prof. Juan J. Alonso's group at Stanford University.
 *                 Prof. Piero Colonna's group at Delft University of Technology.
 *                 Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *                 Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *                 Prof. Rafael Palacios' group at Imperial College London.
 *
 * Copyright (C) 2012-2015 SU2, the open-source CFD code.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once


#ifdef HAVE_MPI
inline void CMPIWrapper::Init(int *argc, char ***argv){
  MPI_Init(argc,argv);
}

inline void CMPIWrapper::Isend(void *buf, int count, MPI_Datatype datatype,
                               int dest, int tag, MPI_Comm comm, MPI_Request *request){
  MPI_Isend(buf,count,datatype,dest,tag,comm,request);
}

inline void CMPIWrapper::Irecv(void *buf, int count, MPI_Datatype datatype,
                               int dest, int tag, MPI_Comm comm, MPI_Request *request){
  MPI_Irecv(buf,count,datatype,dest,tag,comm,request);
}

inline void CMPIWrapper::Wait(MPI_Request *request, MPI_Status *status){
  MPI_Wait(request,status);
}

inline void CMPIWrapper::Waitall(int nrequests, MPI_Request *request, MPI_Status *status){
  MPI_Waitall(nrequests,request,status);
}

inline void CMPIWrapper::Send(void *buf, int count, MPI_Datatype datatype,
                              int dest, int tag, MPI_Comm comm){
  MPI_Send(buf,count,datatype,dest,tag,comm);
}

inline void CMPIWrapper::Recv(void *buf, int count, MPI_Datatype datatype,
                              int dest,int tag, MPI_Comm comm, MPI_Status *status){
  MPI_Recv(buf,count,datatype,dest,tag,comm,status);
}

inline void CMPIWrapper::Bcast(void *buf, int count, MPI_Datatype datatype,
                               int root, MPI_Comm comm){
  MPI_Bcast(buf,count,datatype,root,comm);
}

inline void CMPIWrapper::Bsend(void *buf, int count, MPI_Datatype datatype,
                               int dest, int tag, MPI_Comm comm){
  MPI_Bsend(buf,count,datatype,dest,tag,comm);
}

inline void CMPIWrapper::Reduce(void *sendbuf, void *recvbuf, int count,
                                MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm){
  MPI_Reduce(sendbuf, recvbuf,count,datatype,op,root,comm);
}

inline void CMPIWrapper::Allreduce(void *sendbuf, void *recvbuf, int count,
                                   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
  MPI_Allreduce(sendbuf,recvbuf,count,datatype,op,comm);
}

inline void CMPIWrapper::Gather(void *sendbuf, int sendcnt,MPI_Datatype sendtype,
                                void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm){
  MPI_Gather(sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,root,comm);
}

inline void CMPIWrapper::Allgather(void *sendbuf, int sendcnt, MPI_Datatype sendtype,
                                   void *recvbuf, int recvcnt, MPI_Datatype recvtype, MPI_Comm comm){
  MPI_Allgather(sendbuf,sendcnt,sendtype, recvbuf, recvcnt, recvtype, comm);
}

inline void CMPIWrapper::Sendrecv(void *sendbuf, int sendcnt, MPI_Datatype sendtype,
                                  int dest, int sendtag, void *recvbuf, int recvcnt,
                                  MPI_Datatype recvtype,int source, int recvtag,
                                  MPI_Comm comm, MPI_Status *status){
  MPI_Sendrecv(sendbuf,sendcnt,sendtype,dest,sendtag,recvbuf,recvcnt,recvtype,source,recvtag,comm,status);
}
#ifdef COMPLEX_TYPE
inline void CComplexMPIWrapper::Allgather(void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, MPI_Comm comm){
  if (sendtype != MPI_DOUBLE){
    MPI_Allgather(sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,comm);
  } else {
    int size;
    MPI_Comm_size(comm,&size);
    Gather(sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,0,comm);
    Bcast(recvbuf,recvcnt*size,recvtype,0,comm);
  }
}

inline void CComplexMPIWrapper::Allreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
  if (datatype != MPI_DOUBLE){
    MPI_Allreduce(sendbuf,recvbuf,count,datatype,op,comm);
  } else {
    Reduce(sendbuf,recvbuf, count,datatype,op,0,comm);
    Bcast(recvbuf,count,datatype,0,comm);
  }
}

inline void CComplexMPIWrapper::Sendrecv(void *sendbuf, int sendcnt, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcnt, MPI_Datatype recvtype,int source, int recvtag, MPI_Comm comm, MPI_Status *status){
  if (sendtype != MPI_DOUBLE){
    MPI_Sendrecv(sendbuf,sendcnt,sendtype,dest,sendtag,recvbuf,recvcnt,recvtype,source,recvtag,comm,status);
  } else {
    Bsend(sendbuf,sendcnt,sendtype,dest,sendtag,comm);
    Recv(recvbuf,recvcnt,recvtype,source,recvtag,comm,status);
  }
}

#endif
#endif