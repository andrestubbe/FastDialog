package fastdialog;

/**
 * Result returned by {@link FastDialog#promptCredentials}.
 */
public class DialogCredentials {
    private final String username;
    private final String password;

    public DialogCredentials(String username, String password) {
        this.username = username;
        this.password = password;
    }

    /**
     * The entered username (may contain DOMAIN\\user or user@domain).
     *
     * @return Username string.
     */
    public String getUsername() {
        return username;
    }

    /**
     * The entered password.
     *
     * @return Password string.
     */
    public String getPassword() {
        return password;
    }

    @Override
    public String toString() {
        return "DialogCredentials{username='" + username + "', password='***'}";
    }
}
