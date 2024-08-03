/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:20:27 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/06/03 11:20:28 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

// int main(int argc, char *argv[]) {
//     (void)argc;
//   // Test case 1: No arguments (exit with default status)
//   ft_exit(argv, 0);  // Replace 0 with your desired default status

//   // Test case 2: Exit with specific status (write for visibility)
//   write(1, "\n\n", 2);  // Add newline for better separation
//   ft_exit(argv, 42);

//   // Test case 3: Non-numeric argument (should trigger error)
//   write(1, "\n\n", 2);
//   ft_exit((char *[]) {"exit", "hello", NULL}, 0);  // Invalid usage

//   // This part won't be executed because the program exits in previous calls
//   return (0);
// }