/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 10:15:25 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/12 16:27:47 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void		sleep(size_t time)
{
	int dreams = 1000000;

	time *= 20000000;
	for (size_t i = 0; i < time; i++)
	{
		dreams += 1408;
		dreams /= 1132;
		dreams *= 21;
	}
}

size_t		strlen(const char *str)
{
	size_t len = 0;

	while (str[len])
		len++;
	return len;
}
