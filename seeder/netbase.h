// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2016-2018 The CellLink developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef CELLLINK_NETBASE_H
#define CELLLINK_NETBASE_H

#include <string>
#include <vector>

#include "serialize.h"
#include "compat.h"

extern int nConnectTimeout;

#ifdef _WIN32
// In MSVC, this is defined as a macro, undefine it to prevent a compile and link error
#undef SetPort
#endif

enum Network
{
    NET_UNROUTABLE,
    NET_IPV4,
    NET_IPV6,
    NET_TOR,
    NET_I2P,

    NET_MAX,
};

extern int nConnectTimeout;
extern bool fNameLookup;

/** IP address (IPv6, or IPv4 using mapped IPv6 range (::FFFF:0:0/96)) */
class CellNetAddr
{
    protected:
        unsigned char ip[16]; // in network byte order

    public:
        CellNetAddr();
        CellNetAddr(const struct in_addr& ipv4Addr);
        explicit CellNetAddr(const char *pszIp, bool fAllowLookup = false);
        explicit CellNetAddr(const std::string &strIp, bool fAllowLookup = false);
        void Init();
        void SetIP(const CellNetAddr& ip);
        bool SetSpecial(const std::string &strName); // for Tor and I2P addresses
        bool IsIPv4() const;    // IPv4 mapped address (::FFFF:0:0/96, 0.0.0.0/0)
        bool IsIPv6() const;    // IPv6 address (not mapped IPv4, not Tor/I2P)
        bool IsReserved() const; // Against Hetzners Abusal/Netscan Bot
        bool IsRFC1918() const; // IPv4 private networks (10.0.0.0/8, 192.168.0.0/16, 172.16.0.0/12)
        bool IsRFC3849() const; // IPv6 documentation address (2001:0DB8::/32)
        bool IsRFC3927() const; // IPv4 autoconfig (169.254.0.0/16)
        bool IsRFC3964() const; // IPv6 6to4 tunnelling (2002::/16)
        bool IsRFC4193() const; // IPv6 unique local (FC00::/15)
        bool IsRFC4380() const; // IPv6 Teredo tunnelling (2001::/32)
        bool IsRFC4843() const; // IPv6 ORCHID (2001:10::/28)
        bool IsRFC4862() const; // IPv6 autoconfig (FE80::/64)
        bool IsRFC6052() const; // IPv6 well-known prefix (64:FF9B::/96)
        bool IsRFC6145() const; // IPv6 IPv4-translated address (::FFFF:0:0:0/96)
        bool IsTor() const;
        bool IsI2P() const;
        bool IsLocal() const;
        bool IsRoutable() const;
        bool IsValid() const;
        bool IsMulticast() const;
        enum Network GetNetwork() const;
        std::string ToString() const;
        std::string ToStringIP() const;
        unsigned int GetByte(int n) const;
        uint64 GetHash() const;
        bool GetInAddr(struct in_addr* pipv4Addr) const;
        std::vector<unsigned char> GetGroup() const;
        int GetReachabilityFrom(const CellNetAddr *paddrPartner = NULL) const;
        void print() const;

        CellNetAddr(const struct in6_addr& pipv6Addr);
        bool GetIn6Addr(struct in6_addr* pipv6Addr) const;

        friend bool operator==(const CellNetAddr& a, const CellNetAddr& b);
        friend bool operator!=(const CellNetAddr& a, const CellNetAddr& b);
        friend bool operator<(const CellNetAddr& a, const CellNetAddr& b);

        IMPLEMENT_SERIALIZE
            (
             READWRITE(FLATDATA(ip));
            )
};

/** A combination of a network address (CellNetAddr) and a (TCP) port */
class CellService : public CellNetAddr
{
    protected:
        unsigned short port; // host order

    public:
        CellService();
        CellService(const CellNetAddr& ip, unsigned short port);
        CellService(const struct in_addr& ipv4Addr, unsigned short port);
        CellService(const struct sockaddr_in& addr);
        explicit CellService(const char *pszIpPort, int portDefault, bool fAllowLookup = false);
        explicit CellService(const char *pszIpPort, bool fAllowLookup = false);
        explicit CellService(const std::string& strIpPort, int portDefault, bool fAllowLookup = false);
        explicit CellService(const std::string& strIpPort, bool fAllowLookup = false);
        void Init();
        void SetPort(unsigned short portIn);
        unsigned short GetPort() const;
        bool GetSockAddr(struct sockaddr* paddr, socklen_t *addrlen) const;
        bool SetSockAddr(const struct sockaddr* paddr);
        friend bool operator==(const CellService& a, const CellService& b);
        friend bool operator!=(const CellService& a, const CellService& b);
        friend bool operator<(const CellService& a, const CellService& b);
        std::vector<unsigned char> GetKey() const;
        std::string ToString() const;
        std::string ToStringPort() const;
        std::string ToStringIPPort() const;
        void print() const;

        CellService(const struct in6_addr& ipv6Addr, unsigned short port);
        CellService(const struct sockaddr_in6& addr);

        IMPLEMENT_SERIALIZE
            (
             CellService* pthis = const_cast<CellService*>(this);
             READWRITE(FLATDATA(ip));
             unsigned short portN = htons(port);
             READWRITE(portN);
             if (fRead)
                 pthis->port = ntohs(portN);
            )
};

enum Network ParseNetwork(std::string net);
void SplitHostPort(std::string in, int &portOut, std::string &hostOut);
bool SetProxy(enum Network net, CellService addrProxy, int nSocksVersion = 5);
bool GetProxy(enum Network net, CellService &addrProxy);
bool IsProxy(const CellNetAddr &addr);
bool SetNameProxy(CellService addrProxy, int nSocksVersion = 5);
bool GetNameProxy();
bool LookupHost(const char *pszName, std::vector<CellNetAddr>& vIP, unsigned int nMaxSolutions = 0, bool fAllowLookup = true);
bool LookupHostNumeric(const char *pszName, std::vector<CellNetAddr>& vIP, unsigned int nMaxSolutions = 0);
bool Lookup(const char *pszName, CellService& addr, int portDefault = 0, bool fAllowLookup = true);
bool Lookup(const char *pszName, std::vector<CellService>& vAddr, int portDefault = 0, bool fAllowLookup = true, unsigned int nMaxSolutions = 0);
bool LookupNumeric(const char *pszName, CellService& addr, int portDefault = 0);
bool ConnectSocket(const CellService &addr, SOCKET& hSocketRet, int nTimeout = nConnectTimeout);
bool ConnectSocketByName(CellService &addr, SOCKET& hSocketRet, const char *pszDest, int portDefault = 0, int nTimeout = nConnectTimeout);

#endif
