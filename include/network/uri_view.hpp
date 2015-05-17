#ifndef NETWORK_URI_VIEW_HPP
#define NETWORK_URI_VIEW_HPP

#include <boost/utility/string_ref.hpp>

namespace network
{

class uri_view
{
public:
    typedef boost::string_ref string_view;
    typedef string_view::size_type size_type;
    typedef string_view::value_type value_type;

    uri_view(const string_view&);

    const string_view& scheme() const;
    const string_view& userinfo() const;
    const string_view& host() const;
    const string_view& port() const;

private:
    void parse(string_view);
    size_type parse_scheme(string_view);
    size_type parse_hier_part(string_view);
    size_type parse_authority(string_view);
    size_type parse_userinfo(string_view);
    size_type parse_host(string_view);
    size_type parse_ipv4address(string_view);
    size_type parse_dec_octet(string_view);
    size_type parse_port(string_view);

    bool is_alpha(value_type) const;
    bool is_digit(value_type) const;
    bool is_scheme(value_type) const;

private:
    string_view scheme_view;
    string_view userinfo_view;
    string_view host_view;
    string_view port_view;
};

} // namespace network

#include <network/detail/uri_view.ipp>

#endif // NETWORK_URI_VIEW_HPP
