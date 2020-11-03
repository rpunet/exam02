/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpunet   <rpunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:35:40 by rpunet            #+#    #+#             */
/*   Updated: 2020/08/05 21:03:10 by rpunet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>

static void	ft_putnbr(long nbr, int base_len, char *base)
{
	if (nbr >= base_len)
		ft_putnbr(nbr / base_len, base_len, base);
	write(1, &base[nbr % base_len], 1);
}

static int	ft_nbrlen(long nbr, int base_len)
{
	int	i;

	i = 1;
	while (nbr >= base_len)
	{
		nbr /= base_len;
		i++;
	}
	return (i);
}

int			ft_printf(const char *format, ...)
{
	va_list	ap;
	char	*str;
	int		ret;
	int		width;
	int		prec;
	char	*s;
	int		len;
	long	nbr;
	int		neg;

	va_start(ap, format);
	str = (char *)format;
	ret = 0;
	while (*str)
	{
		if (*str != '%')
			ret += write(1, str, 1);
		else
		{
			str++;
			width = 0;
			prec = -1;
			len = 0;
			neg = 0;
			while (*str >= '0' && *str <= '9')
			{
				width = width * 10 + (*str - '0');
				str++;
			}
			if (*str == '.')
			{
				prec = 0;
				str++;
				while (*str >= '0' && *str <= '9')
				{
					prec = prec * 10 + (*str - '0');
					str++;
				}
			}
			if (*str == 's')
			{
				s = va_arg(ap, char*);
				if (!s)
					s = "(null)";
				while (s[len])
					len++;
				if (prec >= 0 && prec < len)
					len = prec;
				while (width-- > len)
					ret += write(1, " ", 1);
				ret += write(1, s, len);
			}
			if (*str == 'd')
			{
				nbr = va_arg(ap, int);
				if (nbr < 0)
				{
					neg = 1;
					nbr = -nbr;
				}
				len = ft_nbrlen(nbr, 10) + neg;
				if (nbr == 0 && prec == 0)
					len = 0;
				while (width-- > (prec > (len - neg) ? (prec + neg) : len))
					ret += write(1, " ", 1);
				if (neg == 1)
					write (1, "-", 1);
				while (prec-- > (len - neg))
					ret += write(1, "0", 1);
				if (!(nbr == 0 && prec == -1))
					ft_putnbr(nbr, 10, "0123456789");
				ret += len;
			}
			if (*str == 'x')
			{
				nbr = va_arg(ap, unsigned);
				len = ft_nbrlen(nbr, 16);
				if (nbr == 0 && prec == 0)
					len = 0;
				while (width-- > (prec > len ? prec : len))
					ret += write(1, " ", 1);
				while (prec-- > len)
					ret += write(1, "0", 1);
				if (!(nbr == 0 && prec == -1))
					ft_putnbr(nbr, 16, "0123456789abcdef");
				ret += len;
			}
		}
		str++;
	}
	va_end(ap);
	return (ret);
}
